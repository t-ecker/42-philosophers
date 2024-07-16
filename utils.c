#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	minus;
	int	number;

	i = 0;
	minus = 1;
	number = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			minus = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		number *= 10;
		number += (str[i] - 48);
		i++;
	}
	return (number * minus);
}

long	current_time_in_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_usleep(int time)
{
	long	start_time;

	start_time = current_time_in_ms();
	while (current_time_in_ms() - start_time < time)
		usleep(500);
}

void	write_message(char *str, t_data *data, int num)
{
	if (check_death_status(data) == 0)
	{
		pthread_mutex_lock(&data->write_m);
		printf("%ld %i %s\n",
			current_time_in_ms() - data->start_time, num, str);
		pthread_mutex_unlock(&data->write_m);
	}
}
