#include "../philo.h"

int	is_all_num(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	check_args(char **argv, int argc)
{
	if (argc < 5 || argc > 6)
		return (write(2, "Invalid number of arguments\n", 30), 1);
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

void	free_all(t_philo *philos, pthread_mutex_t *forks, pthread_t *threads)
{
	if (philos)
		free(philos);
	if (forks)
		free(forks);
	if (threads)
		free(threads);
}

int	main2(int argc, char **argv)
{
	t_data			data;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_t		*threads;

	init_data(&data, argv, argc);
	philos = malloc(data.philo_count * sizeof(t_philo));
	forks = malloc(data.philo_count * sizeof(pthread_mutex_t));
	threads = malloc(data.philo_count * sizeof(pthread_t));
	if (!philos || !forks || !threads)
		return (free_all(philos, forks, threads), 1);
	init_philos(&data, philos, threads, forks);
	data.philo = philos;
	if (init_fork_mutex(forks, data))
		return (write(2, "init_fork failed\n", 17), free_all(philos, forks, threads), 1);
	if (init_threads(philos))
		return (write(2, "init_threads failed\n", 20), free_all(philos, forks, threads), 1);
	if (join_threads(philos))
		return (write(2, "join_threads failed\n", 20), free_all(philos, forks, threads), 1);
	destroy_forks(forks, data);
	return (free_all(philos, forks, threads), 0);
}

int	main(int argc, char **argv)
{
	if (check_args(argv, argc))
		return (1);
	return (main2(argc, argv));
}
