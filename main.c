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

int	check_death_status(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->stop_m);
	status = data->stop;
	pthread_mutex_unlock(&data->stop_m);
	return (status);
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

void	write_message(char *str, t_data *data, int num)
{
	if (check_death_status(data) == 0)
	{
		pthread_mutex_lock(&data->write_m);
		printf("%ld %i %s\n", current_time_in_ms() - data->start_time, num, str);
		pthread_mutex_unlock(&data->write_m);
	}
}

void ft_usleep(int time)
{
	long start_time;
	start_time = current_time_in_ms();
    while (current_time_in_ms() - start_time < time)
        usleep(500);
}

void routine(t_philo *philo)
{
	write_message("is thinking", philo->data, philo->num);

	pthread_mutex_lock(philo->left_f);
	write_message("has taken a fork", philo->data, philo->num);
	if (philo->data->philo_count == 1)
	{
		ft_usleep(philo->data->time_to_die);
		pthread_mutex_unlock(philo->left_f);
		return ;
	}
	pthread_mutex_lock(philo->right_f);
	write_message("has taken a fork", philo->data, philo->num);
	philo->eating = 1;
	write_message("is eating", philo->data, philo->num);
	pthread_mutex_lock(&philo->data->eating);
	philo->last_meal = current_time_in_ms();
	philo->meal_count++;
	pthread_mutex_unlock(&philo->data->eating);
	ft_usleep(philo->data->time_to_eat);
	philo->eating = 0;
	pthread_mutex_unlock(philo->right_f);
	pthread_mutex_unlock(philo->left_f);

	write_message("is sleeping", philo->data, philo->num);
	ft_usleep(philo->data->time_to_sleep);
}

int check_meals_eaten(t_data *data)
{
    int i;
    int finished;

    i = 0;
    finished = 0;
    while (i < data->philo_count)
    {
        pthread_mutex_lock(&data->eating);
        if (data->max_meals != -1 && data->philo[i].meal_count >= data->max_meals)
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

int check_death(t_data *data)
{
    int i;

    i = 0;
    while (i < data->philo_count)
    {
        pthread_mutex_lock(&data->eating);
        if (current_time_in_ms() - data->philo[i].last_meal > data->time_to_die && data->philo[i].eating == 0)
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

void *monitor(void *arg)
{
	t_data *data;

	data = (t_data *)arg;
	while(check_death_status(data) == 0)
	{
		if (check_death(data) || check_meals_eaten(data))
			break ;
	}
	return (NULL);
}

void *philo_routine(void *arg)
{
    t_philo *philo;

	philo = (t_philo *)arg;
	if (philo->num % 2 != 0)
		usleep(500);
	while(check_death_status(philo->data) == 0)
	{
		routine(philo);
	}
	return (NULL);
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

void init_threads(t_philo *philo)
{
	int i;
	pthread_t	t_observ;

	i = 0;
	pthread_create(&t_observ, NULL, monitor, philo->data);
	while (i < philo->data->philo_count)
	{
		pthread_create(&philo[i].thread, NULL, philo_routine, &philo[i]);
		i++;
	}
	pthread_join(t_observ, NULL);
}
 
void join_threads(t_philo *philo)
{
	int i;

	i = 0;
	while (i < philo->data->philo_count)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
}

int main2(int argc, char **argv)
{
	t_data data;
	t_philo philos[ft_atoi(argv[1])];
	pthread_mutex_t forks[ft_atoi(argv[1])];
	pthread_t threads[ft_atoi(argv[1])];

	init_data(&data, argv, argc);
	init_philos(&data, philos, threads, forks);
	data.philo = philos;
	init_fork_mutex(forks, data);
	init_threads(philos);
	join_threads(philos);
	destroy_forks(forks, data);
	pthread_mutex_destroy(&data.stop_m);
	pthread_mutex_destroy(&data.eating);
	pthread_mutex_destroy(&data.write_m);
	return (0);
}

int main(int argc, char **argv)
{
	if (check_args(argv, argc))
		return (1);
	return (main2(argc, argv));
}
	