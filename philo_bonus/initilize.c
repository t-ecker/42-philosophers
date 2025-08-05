/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initilize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:06:23 by tomecker          #+#    #+#             */
/*   Updated: 2025/08/05 20:32:13 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

int	create_semaphores(t_data *data)
{
	sem_unlink("/write");
	sem_unlink("/max_meals");
	sem_unlink("/forks");
	sem_unlink("/access");
	sem_unlink("/terminate");
	sem_unlink("/mainStop_lock");
	data->write = sem_open("/write", O_CREAT | O_EXCL, 0644, 1);
	data->main_stop_lock = sem_open("/mainStop_lock", \
		O_CREAT | O_EXCL, 0644, 1);
	data->max_meals_sem = sem_open("/max_meals", O_CREAT | O_EXCL, 0644, 0);
	data->terminate = sem_open("/terminate", O_CREAT | O_EXCL, 0644, 0);
	data->forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, data->philo_count);
	data->access = sem_open("/access", O_CREAT | O_EXCL, \
		0644, data->philo_count - 1);
	if (data->write == SEM_FAILED || data->forks == SEM_FAILED
		|| data->access == SEM_FAILED || data->terminate == SEM_FAILED
		|| data->main_stop_lock == SEM_FAILED
		|| data->max_meals_sem == SEM_FAILED)
		return (1);
	return (0);
}

int	init_data(t_data *data, char **argv, int argc)
{
	memset(data, 0, sizeof(t_data));
	data->philo_count = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->max_meals = ft_atoi(argv[5]);
	else
		data->max_meals = -1;
	return (create_semaphores(data));
}

int	init_philos(t_data *data)
{
	pid_t	pid;
	int		i;

	i = -1;
	while (++i < data->philo_count)
	{
		pid = fork();
		if (pid < 0)
			return (1);
		if (pid == 0)
		{
			data->id = i + 1;
			philo_routine(data);
		}
	}
	return (0);
}
