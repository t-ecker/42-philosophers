/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:16:44 by tomecker          #+#    #+#             */
/*   Updated: 2025/08/07 23:48:00 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

void	check_meals_eaten(t_philo *philo)
{
	sem_wait(philo->eating_mutex);
	if (philo->data->max_meals > 0
		&& (philo->meal_count >= philo->data->max_meals)
		&& !philo->has_finished)
	{
		sem_post(philo->data->finished_eating);
		sem_wait(philo->shutdown_mutex);
		philo->has_finished = true;
		philo->status_code = EXIT_MAX_MEALS;
		sem_post(philo->shutdown_mutex);
	}
	sem_post(philo->eating_mutex);
}

void	*check_all_meals_eaten(void *args)
{
	t_data	*data;
	int		finished_eating;
	int		i;
	bool	res;

	i = -1;
	finished_eating = 0;
	data = args;
	while (++i < data->philo_count)
	{
		sem_wait(data->finished_eating);
		sem_wait(data->cleanup_mutex);
		res = data->shutdown;
		sem_post(data->cleanup_mutex);
		if (res)
			return (NULL);
		if (++finished_eating >= data->philo_count)
		{
			i = -1;
			while (++i < data->philo_count)
				sem_post(data->terminate);
			break ;
		}
	}
	return (NULL);
}

void	handle_death(t_philo *philo)
{
	int		i;

	i = -1;
	sem_wait(philo->data->death_mutex);
	if (!check_shutdown(philo))
	{
		write_message("died", philo->data, philo->num);
		while (++i < philo->data->philo_count)
			sem_post(philo->data->terminate);
	}
	sem_post(philo->data->death_mutex);
	sem_wait(philo->shutdown_mutex);
	philo->data->shutdown = true;
	philo->status_code = EXIT_DIED;
	sem_post(philo->shutdown_mutex);
}

void	*check_own_death(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!check_shutdown(philo))
	{
		sem_wait(philo->eating_mutex);
		if (current_time_in_ms() - philo->last_meal > philo->data->time_to_die)
		{
			handle_death(philo);
			sem_post(philo->eating_mutex);
			break ;
		}
		sem_post(philo->eating_mutex);
		usleep(philo->data->philo_count * 50);
	}
	return (NULL);
}

bool	check_shutdown(t_philo *philo)
{
	bool	status;

	sem_wait(philo->shutdown_mutex);
	status = philo->data->shutdown;
	sem_post(philo->shutdown_mutex);
	return (status);
}
