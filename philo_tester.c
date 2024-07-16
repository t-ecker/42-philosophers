#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 1024
#define ERROR "❌"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	size_t	i;
	size_t	j;
	char	*ptr;

	i = 0;
	j = 0;
	len1 = strlen(s1);
	len2 = strlen(s2);
	ptr = malloc(sizeof(char) * (len1 + len2 + 1));
	if (ptr == NULL)
		return (NULL);
	while (i < len1)
	{
		ptr[i] = s1[i];
		i++;
	}
	while (j < len2)
	{
		ptr[i + j] = s2[j];
		j++;
	}
	ptr[i + j] = '\0';
	return (ptr);
}

char	*get_buffer(void)
{
	char	*buffer;
	char	tmp_buffer[BUFFERSIZE];
	int		total_size;
	char	*new_buffer;
	int		bytesread;

	total_size = 0;
	buffer = NULL;
	while (fgets(tmp_buffer, sizeof(tmp_buffer), stdin) != NULL)
	{
		bytesread = strlen(tmp_buffer);
		new_buffer = realloc(buffer, total_size + bytesread + 1);
		if (!new_buffer)
		{
			free(buffer);
			perror("Failed to reallocate memory");
			return (NULL);
		}
		buffer = new_buffer;
		memcpy(buffer + total_size, tmp_buffer, bytesread);
		total_size += bytesread;
	}
	if (buffer)
		buffer[total_size] = '\0';
	return (buffer);
}

void	process(char *buffer, int num)
{
	int		i;
	int		count;
	char	*tmp;
	char	c[2];
	char	*str;
	int		flag;

	flag = 0;
	i = 1;
	printf("\n");
	while (i <= num)
	{
		c[0] = i + '0';
		c[1] = '\0';
		count = 0;
		str = ft_strjoin(c, " is eating");
		tmp = buffer;
		while ((tmp = strstr(tmp, str)))
		{
			tmp += strlen(str);
			count++;
		}
		printf("philo %i has eaten %i times!\n", i, count);
		free(str);
		str = ft_strjoin(c, "died");
		if (strstr(buffer, "died"))
			flag = i;
		free(str);
		i++;
	}
	if (flag)
		printf("\n%s \033[1;31m%i died!\033[0m\n", ERROR, flag);
	printf("\n");
}

int	main(int argc, char **argv)
{
	char	*buffer;

	if (argc != 2)
		return (write(2, "wrong arguments!\n", 17), 1);
	buffer = get_buffer();
	if (!buffer)
		return (1);
	process(buffer, atoi(argv[1]));
	free(buffer);
}
