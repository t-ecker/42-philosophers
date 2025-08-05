/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:16:44 by tomecker          #+#    #+#             */
/*   Updated: 2025/08/05 20:25:16 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

void	check_meals_eaten(t_philo *philo)
{
	sem_wait(philo->eating_lock);
	if (philo->data->max_meals > 0
		&& (philo->meal_count >= philo->data->max_meals)
		&& !philo->has_finished)
	{
		sem_post(philo->data->max_meals_sem);
		sem_wait(philo->shutdown_lock);
		philo->shutdown = true;
		philo->has_finished = true;
		philo->status_code = EXIT_MAX_MEALS;
		sem_post(philo->shutdown_lock);
	}
	sem_post(philo->eating_lock);
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
		sem_wait(data->max_meals_sem);
		sem_wait(data->main_stop_lock);
		res = data->main_stop;
		sem_post(data->main_stop_lock);
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

void	*check_own_death(void *arg)
{
	t_philo	*philo;
	int		i;

	i = -1;
	philo = (t_philo *)arg;
	while (!check_shutdown(philo))
	{
		sem_wait(philo->eating_lock);
		if (current_time_in_ms() - philo->last_meal > philo->data->time_to_die)
		{
			write_message("died", philo->data, philo->num, true);
			sem_wait(philo->shutdown_lock);
			philo->shutdown = true;
			philo->status_code = EXIT_DIED;
			while (++i < philo->data->philo_count)
				sem_post(philo->data->terminate);
			sem_post(philo->shutdown_lock);
			sem_post(philo->eating_lock);
			break ;
		}
		sem_post(philo->eating_lock);
		usleep(philo->data->philo_count * 50);
	}
	return (NULL);
}

bool	check_shutdown(t_philo *philo)
{
	bool	status;

	sem_wait(philo->shutdown_lock);
	status = philo->shutdown;
	sem_post(philo->shutdown_lock);
	return (status);
}
