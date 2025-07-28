#include "./philo.h"

int	init_data(t_data *data, char **argv, int argc)
{
	if (!data)
		return (1);
	data->philo_count = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->max_meals = ft_atoi(argv[5]);
	else
		data->max_meals = -1;
	data->start_time = current_time_in_ms();
	data->philo = NULL;
	sem_unlink("/write");
	sem_unlink("/forks");
	// sem_unlink("/allCreated");
	data->write = sem_open("/write", O_CREAT | O_EXCL, 0644, 1);
	// data->allCreated = sem_open("/allCreated", O_CREAT | O_EXCL, 0644, 0);
	data->forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, data->philo_count);

	if (data->write == SEM_FAILED || data->forks == SEM_FAILED)
		return 1;
	return (0);
}
#include <string.h>
int init_philo_data(t_philo *philo, int index)
{
	char *semName;

	semName = make_unique_semName("eating_", index);
	if (!semName)
		return (1);
	philo->num = index + 1;
	philo->meal_count = 0;
	sem_unlink(semName);
	philo->eating = sem_open(semName, O_CREAT | O_EXCL, 0644, 1);
	free(semName);
	if (philo->eating == SEM_FAILED)
	{
		write(1, "pp\n", 3);
		return (1);
	}
	return (0);
}

int	init_philos(t_data *data, t_philo *philo)
{
	int	i;
	pid_t pid;

	if (!philo)
		return (1);
	i = -1;
	// printf("was: %i", ++i);
	while (++i < data->philo_count)
	{
		if (init_philo_data(&philo[i], i))
			return (1);
		philo[i].data = data;

		pid = fork();
		if (pid < 0)
			return 1;
		if (pid == 0)
		{
			// write(1, "as\n", 3);
			philo_routine(&philo[i]);
			exit(EXIT_SUCCESSFUL);
		}
		philo[i].pid = pid;
	}
	// i = -1;
			// write(1, "yy\n", 3);
	// while(++i < data->philo_count)
	// {
	// 	sem_post(data->allCreated);
	// }
	return (0);
}
