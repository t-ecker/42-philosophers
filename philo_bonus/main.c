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
	if (data->forks)
	{
		sem_close(data->forks);
		sem_unlink("/forks");
	}
	if (data->write)
	{
		sem_close(data->write);
		sem_unlink("/write");
	}
	if (data->access)
	{
		sem_close(data->access);
		sem_unlink("/access");
	}
	if (data->terminate)
	{
		sem_close(data->terminate);
		sem_unlink("/terminate");
	}
}

void waitForProcesses(t_data *data)
{
	int i;
	int status;
	int finishedEating;
	
	i = -1;
	finishedEating = 0;
	while(1)
	{
		waitpid(-1, &status, 0);
		status = WEXITSTATUS(status);
		if (status == EXIT_DIED || status == EXIT_ERROR)
		{
			while(++i < data->philo_count - 1)
				sem_post(data->terminate);
			break;
		}
		if (status == EXIT_MAX_MEALS)
		{
			if (++finishedEating >= data->philo_count)
				break;
		}
	}
}

int	main(int argc, char **argv)
{
	t_data			data;

	if (check_args(argv, argc))
		return (1);
	if (init_data(&data, argv, argc) || init_philos(&data))
		return (cleanup(&data), 1);
	waitForProcesses(&data);
	cleanup(&data);
	return(0);
}
