# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tecker <tecker@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/16 16:43:55 by tecker            #+#    #+#              #
#    Updated: 2024/07/16 16:50:38 by tecker           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = CC
CFLAGS = -Wall -Werror -Wextra

OBJ =	main.o \
		utils.c \
		initilize.c \
		observer.c \
		philo_routine.c \
		threads.c \

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

all:	$(NAME)

$(NAME):	$(OBJ)
	@$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o
	clear;

fclean:	clean
	rm -f $(NAME)
	clear;

re:				fclean all

.PHONY:			all clean fclean re