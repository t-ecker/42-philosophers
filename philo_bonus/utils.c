#include "./philo.h"

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

void	write_message(char *str, t_data *data, int num, bool stopWriting)
{
	sem_wait(data->write);
	printf("%ld %i %s\n",
		current_time_in_ms() - data->start_time, num, str);
	if (!stopWriting)
		sem_post(data->write);
}

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
		++i;
	}
	return (0);
}

size_t	ft_strlen(char const *src)
{
	size_t	i;
	
	i = 0;
	while (src[i])
	i++;
	return (i);
}

char *make_unique_semName(char *base, int index)
{
	int i;
	int j;
	char *name;

	i = 0;
	j = 0;
	name = malloc(sizeof(char) * (ft_strlen(base) + 4));
	if (!name)
		return NULL;
	while (base[i])
		name[j++] = base[i++];
	if (index >= 100)
		name[j++] = '0' + (index / 100 % 10);
	if (index >= 10)
		name[j++] = '0' + (index / 10 % 10);
	name[j++] = '0' + (index % 10);
	name[j] = '\0';
	return name;
}
