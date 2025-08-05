# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/05 20:36:36 by tomecker          #+#    #+#              #
#    Updated: 2025/08/05 20:36:37 by tomecker         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc
CFLAGS = -Wall -Werror -Wextra

SRC =	./main.c \
		./utils.c \
		./initilize.c \
		./observer.c \
		./philo_routine.c \
		./threads.c

SRC_TESTER = ./philo_tester.c

OBJ_DIR = ./obj

OBJ_FILES = $(SRC:./%.c=$(OBJ_DIR)/%.o)
OBJ_FILES_TESTER = $(SRC_TESTER:./%.c=$(OBJ_DIR)/%.o)

all:	$(NAME)
	clear;
	@$(MAKE) loading
	clear;

$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: ./%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

tester:	$(OBJ_FILES_TESTER)
	$(CC) $(CFLAGS) $^ -o $@
	clear;
	@$(MAKE) loading
	clear;

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) tester

re: fclean all

loading:
	@for i in {1..42}; do \
		printf '%s' "█"; \
		sleep 0.01; \
	done

.PHONY: all clean fclean re loading
