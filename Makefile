NAME = philo

CC = cc
CFLAGS = -Wall -Werror -Wextra

SRC =	src/main.c \
		src/utils.c \
		src/initilize.c \
		src/observer.c \
		src/philo_routine.c \
		src/threads.c

SRC_TESTER = src/philo_tester.c

OBJ_DIR = ./obj

OBJ_FILES = $(SRC:src/%.c=$(OBJ_DIR)/%.o)
OBJ_FILES_TESTER = $(SRC_TESTER:src/%.c=$(OBJ_DIR)/%.o)

all:	$(NAME)
	clear;
	@$(MAKE) loading
	clear;

$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
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
