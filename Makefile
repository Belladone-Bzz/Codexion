SRC =	main.c

SRC +=	src/parser.c src/initialization.c src/time.c src/error.c src/coder.c \
		src/init_utils.c

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
	make fclean
	rm -f $(OBJ)

fclean:
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re