/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:06:32 by tomecker          #+#    #+#             */
/*   Updated: 2025/08/05 20:31:15 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

int	check_args(char **argv, int argc)
{
	if (argc < 5 || argc > 6)
		return (write(2, "Invalid number of arguments\n", 28), 1);
	if (is_all_num(argv[1]) || ft_atoi(argv[1]) <= 0 || ft_atoi(argv[1]) > 200)
		return (write(2, "Wrong amound of philosophers\n", 29), 1);
	if (is_all_num(argv[2]) || ft_atoi(argv[2]) <= 0)
		return (write(2, "Wrong time_to_die\n", 18), 1);
	if (is_all_num(argv[3]) || ft_atoi(argv[3]) <= 0)
		return (write(2, "Wrong time_to_eat\n", 18), 1);
	if (is_all_num(argv[4]) || ft_atoi(argv[4]) <= 0)
		return (write(2, "Wrong time_to_sleep\n", 20), 1);
	if (argc == 6 && (is_all_num(argv[5]) || ft_atoi(argv[5]) <= 0))
		return (write(2, "Wrong meals_to_eat\n", 19), 1);
	return (0);
}

void	cleanup(t_data *data)
{
	close_shared_sems(data);
	sem_unlink("/forks");
	sem_unlink("/write");
	sem_unlink("/access");
	sem_unlink("/mainStop_lock");
	sem_unlink("/terminate");
	sem_unlink("/max_meals");
}

bool	create_meals_eaten_thread(t_data *data,
		pthread_t *check_meals_eaten_thread, int i)
{
	if (data->max_meals > 0)
	{
		if (pthread_create(check_meals_eaten_thread, NULL, \
			check_all_meals_eaten, data) != 0)
		{
			while (++i < data->philo_count)
				sem_post(data->terminate);
			return (false);
		}
	}
	return (true);
}

void	wait_for_processes(t_data *data)
{
	int			i;
	int			status;
	pthread_t	check_meals_eaten_thread;
	bool		mealthread_created;

	i = -1;
	mealthread_created = create_meals_eaten_thread(data, \
		&check_meals_eaten_thread, i);
	waitpid(-1, &status, 0);
	if (data->max_meals > 0 && mealthread_created)
	{
		sem_wait(data->main_stop_lock);
		data->main_stop = true;
		sem_post(data->main_stop_lock);
		sem_post(data->max_meals_sem);
		pthread_join(check_meals_eaten_thread, NULL);
	}
	if (WEXITSTATUS(status) == EXIT_ERROR)
	{
		while (++i < data->philo_count - 1)
			sem_post(data->terminate);
		i = -1;
	}
	while (++i < data->philo_count - 1)
		waitpid(-1, NULL, 0);
}

int	main(int argc, char **argv)
{
	t_data			data;

	if (check_args(argv, argc))
		return (1);
	if (init_data(&data, argv, argc) || init_philos(&data))
		return (cleanup(&data), 1);
	wait_for_processes(&data);
	cleanup(&data);
	return (0);
}
