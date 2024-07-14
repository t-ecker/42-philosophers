#include "philo.h"

long current_time_in_ms(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void init_data(t_data *data, char **argv, int argc)
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
	pthread_mutex_init(&data->stop_m, NULL);
	pthread_mutex_init(&data->write_m, NULL);
	pthread_mutex_init(&data->eating, NULL);
}

void init_philos(t_data *data, t_philo *philo, pthread_t *threads, pthread_mutex_t *forks)
{
	int i;

	i = 0;
	while (i < data->philo_count)
	{
		philo[i].num = i + 1;
		philo[i].data = data;
		philo[i].eat_count = 0;
		philo[i].last_meal = current_time_in_ms();
		philo[i].thread = threads[i];
		philo[i].left_f = forks[i];
		philo[i].right_f = forks[(i + 1) % data->philo_count];
		i++;
	}
}

int is_all_num(char *str)
{
	int i;

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

int check_args(char **argv, int argc)
{
	if (argc < 5 || argc > 6)
        return (write(2, "Invalid number of arguments\n", 30), 1);
	if (is_all_num(argv[1]) || ft_atoi(argv[1]) <= 0 || ft_atoi(argv[1]) > 200)
		return(write(2, "Wrong amound of philosophers\n", 29), 1);
	if (is_all_num(argv[2]) || ft_atoi(argv[2]) <= 0)
		return(write(2, "Wrong time_to_die\n", 18), 1);
	if (is_all_num(argv[3]) || ft_atoi(argv[3]) <= 0)
		return(write(2, "Wrong time_to_eat\n", 18), 1);
	if (is_all_num(argv[4]) || ft_atoi(argv[4]) <= 0)
		return(write(2, "Wrong time_to_sleep\n", 20), 1);
	if (argc == 6 && (is_all_num(argv[5]) || ft_atoi(argv[5]) <= 0))
		return(write(2, "Wrong meals_to_eat\n", 19), 1);
	return (0);
}

void init_fork_mutex(pthread_mutex_t *forks, t_data data)
{
	int i;

	i = 0;
	while (i < data.philo_count)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}

void destroy_forks(pthread_mutex_t *forks, t_data data)
{
	int i;

	i = 0;
	while (i < data.philo_count)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

void	write_message(char *str, t_philo *philo)
{
	if (!philo->data->stop)
	{
		pthread_mutex_lock(&philo->data->write_m);
		printf("%ld %i %s\n", current_time_in_ms() - philo->data->start_time, philo->num, str);
		pthread_mutex_unlock(&philo->data->write_m);
	}
}

void ft_usleep(int time)
{
	long start_time;
	start_time = current_time_in_ms();
    while (current_time_in_ms() - start_time < time)
        usleep(time / 10);
}

void *check_stop(void *arg)
{
    t_philo *philo;

	philo = (t_philo *)arg;
	ft_usleep(philo->data->time_to_die + 1);
	pthread_mutex_lock(&philo->data->eating);
	pthread_mutex_lock(&philo->data->stop_m);
	if (!philo->data->stop && philo->eat_count == philo->data->max_meals || current_time_in_ms() - philo->last_meal >= (long)philo->data->time_to_die)
	{
		pthread_mutex_unlock(&philo->data->stop_m);
		pthread_mutex_unlock(&philo->data->eating);
		write_message("died", philo);
		philo->data->stop = 1;
	}
	pthread_mutex_unlock(&philo->data->stop_m);
	pthread_mutex_unlock(&philo->data->eating);
	return (NULL);
}

// int check_stop(t_philo *philo)
// {
// 	// pthread_mutex_lock(&philo->data->stop_m);
// 	// if (philo->data->stop)
// 	// {
// 	// 	pthread_mutex_unlock(&philo->data->stop_m);
// 	// 	return (1);
// 	// }
// 	// if (philo->eat_count == philo->data->max_meals || current_time_in_ms() - philo->last_meal >= (long)philo->data->time_to_die)
// 	// {
// 	// 	write_message("died", philo);
// 	// 	philo->data->stop = 1;
// 	// 	philo->alive = 0;
// 	// 	pthread_mutex_unlock(&philo->data->stop_m);
// 	// 	return (1);
// 	// }
// 	// pthread_mutex_unlock(&philo->data->stop_m);
// 	return (0);
// }



void eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_f);
	write_message("has taken a fork", philo);
	pthread_mutex_lock(&philo->right_f);
	write_message("has taken a fork", philo);
	write_message("is eating", philo);
	pthread_mutex_lock(&philo->data->eating);
	philo->eat_count++;
	philo->last_meal = current_time_in_ms();
	pthread_mutex_unlock(&philo->data->eating);
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&philo->right_f);
	pthread_mutex_unlock(&philo->left_f);
	write_message("is sleeping", philo);
	ft_usleep(philo->data->time_to_sleep);
	write_message("is thinking", philo);
}

void *philo_routine(void *arg)
{
    t_philo *philo;
	pthread_t t_observ;

	philo = (t_philo *)arg;
	if (philo->num % 2 == 0)
		ft_usleep(philo->data->time_to_eat);
	while(!philo->data->stop)
	{
		pthread_create(&t_observ, NULL, check_stop, philo);
		eating(philo);
		pthread_detach(t_observ);
	}
	return (NULL);
}

void init_threads(t_philo *philo, t_data data)
{
	int i;

	i = 0;
	while (i < data.philo_count)
	{
		pthread_create(&philo[i].thread, NULL, philo_routine, &philo[i]);
		i++;
	}
}

void join_threads(t_philo *philo, t_data data)
{
	int i;

	i = 0;
	while (i < data.philo_count)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
}

int main(int argc, char **argv)
{
	t_data data;
	t_philo philos[ft_atoi(argv[1])];
	pthread_mutex_t forks[ft_atoi(argv[1])];
	pthread_t threads[ft_atoi(argv[1])];

	if (check_args(argv, argc))
		exit(1);
	init_data(&data, argv, argc);
	init_philos(&data, philos, threads, forks);
	init_fork_mutex(forks, data);
	init_threads(philos, data);
	join_threads(philos, data);
	destroy_forks(forks, data);
	pthread_mutex_destroy(&data.stop_m);
	pthread_mutex_destroy(&data.eating);
	pthread_mutex_destroy(&data.write_m);
	return (0);
}
