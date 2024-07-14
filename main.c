#include "philo.h"

long current_time_in_ms(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000 + time.tv_usec / 1000);
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
	return (0);
}