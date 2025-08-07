/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initilize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:06:23 by tomecker          #+#    #+#             */
/*   Updated: 2025/08/07 22:18:53 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

int	create_semaphores(t_data *data)
{
	sem_unlink("/print_mutex");
	sem_unlink("/finished_eating");
	sem_unlink("/fork_pool");
	sem_unlink("/death_mutex");
	sem_unlink("/fork_guard");
	sem_unlink("/terminate");
	sem_unlink("/mainStop_lock");
	data->print_mutex = sem_open("/print_mutex", O_CREAT | O_EXCL, 0644, 1);
	data->death_mutex = sem_open("/death_mutex", O_CREAT | O_EXCL, 0644, 1);
	data->cleanup_mutex = sem_open("/mainStop_lock", \
		O_CREAT | O_EXCL, 0644, 1);
	data->finished_eating = sem_open("/finished_eating", O_CREAT | O_EXCL, 0644, 0);
	data->terminate = sem_open("/terminate", O_CREAT | O_EXCL, 0644, 0);
	data->fork_pool = sem_open("/fork_pool", O_CREAT | O_EXCL, 0644, data->philo_count);
	data->fork_guard = sem_open("/fork_guard", O_CREAT | O_EXCL, \
		0644, data->philo_count - 1);
	if (data->print_mutex == SEM_FAILED || data->fork_pool == SEM_FAILED
		|| data->fork_guard == SEM_FAILED || data->terminate == SEM_FAILED
		|| data->cleanup_mutex == SEM_FAILED
		|| data->finished_eating == SEM_FAILED
		|| data->death_mutex == SEM_FAILED)
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
