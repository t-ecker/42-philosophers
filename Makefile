NAME = philosophers

CC = CC
# CFLAGS = -Wall -Werror -Wextra

SRC =	main.c \
		utils.c

all:	$(NAME)
	clear;
	@$(MAKE) loading
	clear;


$(NAME):	$(SRC)
	@$(CC) $(CFLAGS) -o $@ $^

clean:
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



.PHONY:			all clean fclean re