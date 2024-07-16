/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tecker <tecker@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:44:09 by tecker            #+#    #+#             */
/*   Updated: 2024/07/16 16:44:10 by tecker           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_fork_mutex(pthread_mutex_t *forks, t_data data)
{
	int	i;

	i = 0;
	while (i < data.philo_count)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

void	destroy_forks(pthread_mutex_t *forks, t_data data)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&data.stop_m);
	pthread_mutex_destroy(&data.eating);
	pthread_mutex_destroy(&data.write_m);
	while (i < data.philo_count)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

int	init_threads(t_philo *philo)
{
	int			i;
	pthread_t	t_observ;

	i = 0;
	if (pthread_mutex_init(&philo->data->stop_m, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&philo->data->write_m, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&philo->data->eating, NULL) != 0)
		return (1);
	if (pthread_create(&t_observ, NULL, observer, philo->data) != 0)
		return (1);
	while (i < philo->data->philo_count)
	{
		if (pthread_create(&philo[i].thread, NULL, philo_routine, &philo[i])
			!= 0)
			return (1);
		i++;
	}
	if (pthread_join(t_observ, NULL) != 0)
		return (1);
	return (0);
}

int	join_threads(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->data->philo_count)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}
