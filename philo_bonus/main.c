#include "./philo.h"

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

void kill_all_processes(t_data *data)
{
	int i;
	char *semName;

	i = -1;
	while(++i < data->philo_count) 
	{
		pid_t pid = data->philo[i].pid;
		if (pid > 0)
			kill(pid, SIGKILL);
	}

	i = -1;
	while(++i < data->philo_count) 
	{
		pid_t pid = data->philo[i].pid;
		waitpid(pid, NULL, 0);
		data->philo[i].pid = 0;

		semName = make_unique_semName("eating_", i);
		if (!semName)
			continue;
		sem_close(data->philo[i].eating);
		sem_unlink(semName);
		free(semName);
	}
}

void	cleanup(t_data *data)
{
	if (data->philo)
	{
		kill_all_processes(data);
		free(data->philo);
		data->philo = NULL;
	}

	if (data) {
		if (data->forks)
		{
			sem_close(data->forks);
			sem_unlink("/forks");
			data->forks = NULL;
		}
		if (data->write)
		{
			sem_close(data->write);
			sem_unlink("/write");
			data->write = NULL;
		}

	}
}

void waitForProcesses(t_data *data)
{
	int status;
	int finishedEating;

	finishedEating = 0;
	while(1)
	{
		if (waitpid(-1, &status, 0) < 0)
			break;
		status = WEXITSTATUS(status);
		if (status == EXIT_DIED || status == EXIT_ERROR)
			break;
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
	t_philo			*philos;

	if (check_args(argv, argc))
		return (1);
	if (init_data(&data, argv, argc))
		return (1);
		
	philos = malloc(sizeof(t_philo) * data.philo_count);
	if (!philos)
		return (cleanup(&data), 1);
	data.philo = philos;
	
	if (init_philos(&data, philos))
		return (cleanup(&data), 1);

	waitForProcesses(&data);
	cleanup(&data);
	return(0);
}
