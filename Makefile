SRC =	main.c

SRC +=	src/parser.c src/initialization.c src/time.c src/printer.c src/coder.c \
		src/thread.c src/monitoring.c src/dongles.c

OBJ =	$(SRC:.c=.o)

INC =	-I./include
CFLAGS =-Wall -Wextra -Werror -pthread
CC =	cc
NAME =	codexion

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
