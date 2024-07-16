/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tecker <tecker@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:43:59 by tecker            #+#    #+#             */
/*   Updated: 2024/07/16 16:44:00 by tecker           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_meals_eaten(t_data *data)
{
	int	i;
	int	finished;

	i = 0;
	finished = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&data->eating);
		if (data->max_meals != -1
			&& data->philo[i].meal_count >= data->max_meals)
			finished++;
		pthread_mutex_unlock(&data->eating);
		i++;
	}
	if (finished == data->philo_count)
	{
		pthread_mutex_lock(&data->stop_m);
		data->stop = 1;
		pthread_mutex_unlock(&data->stop_m);
		return (1);
	}
	return (0);
}

int	check_death(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&data->eating);
		if (current_time_in_ms() - data->philo[i].last_meal > data->time_to_die
			&& data->philo[i].eating == 0)
		{
			write_message("died", data, data->philo[i].num);
			pthread_mutex_lock(&data->stop_m);
			data->stop = 1;
			pthread_mutex_unlock(&data->stop_m);
			pthread_mutex_unlock(&data->eating);
			return (1);
		}
		pthread_mutex_unlock(&data->eating);
		i++;
	}
	return (0);
}

void	*observer(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (check_death_status(data) == 0)
	{
		if (check_death(data) || check_meals_eaten(data))
			break ;
	}
	return (NULL);
}
