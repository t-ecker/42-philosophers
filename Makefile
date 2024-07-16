# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tecker <tecker@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/16 16:43:55 by tecker            #+#    #+#              #
#    Updated: 2024/07/16 16:47:51 by tecker           ###   ########.fr        #
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
	clear;
	@$(MAKE) loading
	clear;

$(NAME):	$(OBJ)
	@$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o
	clear;

fclean:	clean
	rm -f $(NAME)
	clear;


re:				fclean all

loading:
				@for i in {1..42}; do \
					printf '%s' "█"; \
					sleep 0.01; \
				done



.PHONY:			all clean fclean re loading