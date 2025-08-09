/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tecker <tecker@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:18:51 by tomecker          #+#    #+#             */
/*   Updated: 2025/08/09 13:02:19 by tecker           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

void	terminate(t_philo *philo, t_exit_code exitCode)
{
	if (philo->check_own_death_thread)
		pthread_join(philo->check_own_death_thread, NULL);
	if (philo->routine_thread)
		pthread_join(philo->routine_thread, NULL);
	close_shared_sems(philo->data);
	if (philo->semname_eating)
	{
		sem_close(philo->eating_mutex);
		sem_unlink(philo->semname_eating);
		free(philo->semname_eating);
		philo->semname_eating = NULL;
	}
	if (philo->semname_shutdown)
	{
		sem_close(philo->shutdown_mutex);
		sem_unlink(philo->semname_shutdown);
		free(philo->semname_shutdown);
		philo->semname_shutdown = NULL;
	}
	if (philo->status_code == EXIT_DIED || philo->status_code == EXIT_MAX_MEALS)
		exitCode = philo->status_code;
	exit(exitCode);
}

void	routine(t_philo *philo)
{
	if (philo->data->philo_count == 1)
		return (ft_usleep(philo->data->time_to_die + 1));
	sem_wait(philo->data->fork_guard);
	sem_wait(philo->data->fork_pool);
	if (check_shutdown(philo))
		return ;
	write_message("has taken a fork", philo->data, philo->num);
	sem_wait(philo->data->fork_pool);
	if (check_shutdown(philo))
		return ;
	write_message("has taken a fork", philo->data, philo->num);
	sem_wait(philo->eating_mutex);
	write_message("is eating", philo->data, philo->num);
	philo->last_meal = current_time_in_ms();
	philo->meal_count++;
	sem_post(philo->eating_mutex);
	check_meals_eaten(philo);
	ft_usleep(philo->data->time_to_eat);
	sem_post(philo->data->fork_pool);
	sem_post(philo->data->fork_pool);
	sem_post(philo->data->fork_guard);
	if (check_shutdown(philo))
		return ;
	write_message("is sleeping", philo->data, philo->num);
	ft_usleep(philo->data->time_to_sleep);
}

void	*routine_thread(void *args)
{
	t_philo	*philo;

	philo = args;
	while (!check_shutdown(philo))
	{
		write_message("is thinking", philo->data, philo->num);
		routine(philo);
	}
	return (NULL);
}

int	init_philo(t_data *data, t_philo *philo)
{
	memset(philo, 0, sizeof(t_philo));
	philo->num = data->id;
	philo->semname_eating = make_unique_semname("eating_", philo->num);
	philo->semname_shutdown = make_unique_semname("shutdown_", philo->num);
	if (!philo->semname_eating || !philo->semname_shutdown)
		return (1);
	sem_unlink(philo->semname_eating);
	sem_unlink(philo->semname_shutdown);
	philo->eating_mutex = sem_open(philo->semname_eating, \
		O_CREAT | O_EXCL, 0644, 1);
	philo->shutdown_mutex = sem_open(philo->semname_shutdown, O_CREAT \
		| O_EXCL, 0644, 1);
	if (philo->eating_mutex == SEM_FAILED
		|| philo->shutdown_mutex == SEM_FAILED)
		return (1);
	philo->data = data;
	philo->last_meal = current_time_in_ms();
	philo->data->start_time = current_time_in_ms();
	if (pthread_create(&philo->check_own_death_thread, NULL, \
		check_own_death, philo) != 0)
		terminate(philo, EXIT_ERROR);
	return (0);
}

void	philo_routine(void *arg)
{
	t_data	*data;
	t_philo	philo;

	data = (t_data *)arg;
	if (init_philo(data, &philo))
	{
		philo.data = data;
		terminate(&philo, EXIT_ERROR);
	}
	if (pthread_create(&philo.routine_thread, NULL, \
		routine_thread, &philo) != 0)
	{
		sem_wait(philo.shutdown_mutex);
		philo.data->shutdown = true;
		sem_post(philo.shutdown_mutex);
		pthread_join(philo.check_own_death_thread, NULL);
		philo.check_own_death_thread = 0;
		terminate(&philo, EXIT_ERROR);
	}
	sem_wait(data->terminate);
	sem_wait(philo.shutdown_mutex);
	philo.data->shutdown = true;
	sem_post(philo.shutdown_mutex);
	terminate(&philo, EXIT_CLOSED);
}
