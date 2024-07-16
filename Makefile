NAME = philo

CC = CC
# CFLAGS = -Wall -Werror -Wextra

OBJ =	main.o \
		utils.c

OBJ_TESTER = philo_tester.c

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

all:	$(NAME)
	clear;
	@$(MAKE) loading
	clear;

tester:	all $(OBJ_TESTER)
	@$(CC) -o $@ $(OBJ_TESTER)

$(NAME):	$(OBJ)
	@$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o *.out
	clear;

fclean:	clean
	rm -f $(NAME) tester
	clear;


re:				fclean all

loading:
				@for i in {1..42}; do \
					printf '%s' "█"; \
					sleep 0.01; \
				done



.PHONY:			all clean fclean re