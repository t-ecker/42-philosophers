/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initilize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tecker <tecker@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:43:48 by tecker            #+#    #+#             */
/*   Updated: 2024/07/16 16:43:49 by tecker           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_data(t_data *data, char **argv, int argc)
{
	(data)->philo_count = ft_atoi(argv[1]);
	(data)->time_to_die = ft_atoi(argv[2]);
	(data)->time_to_eat = ft_atoi(argv[3]);
	(data)->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		(data)->max_meals = ft_atoi(argv[5]);
	else
		(data)->max_meals = -1;
	(data)->stop = 0;
	data->start_time = current_time_in_ms();
}

void	init_philos(t_data *data, t_philo *philo,
	pthread_t *threads, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		philo[i].num = i + 1;
		philo[i].last_meal = current_time_in_ms();
		philo[i].meal_count = 0;
		philo[i].data = data;
		philo[i].eating = 0;
		philo[i].thread = threads[i];
		philo[i].left_f = &forks[i];
		philo[i].right_f = &forks[(i + 1) % data->philo_count];
		i++;
	}
}
