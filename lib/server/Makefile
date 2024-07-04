##
## FIRST PERSONAL PROJECT, 2024
## TekosKart
## File description:
## Makefile
##

SRC	=	$(wildcard src/main.c) \
		$(wildcard src/game/*.c) \
		$(wildcard src/misc/*.c)

NAME	=	server

INCLUDES	=	include

ifeq	($(shell uname -s),Darwin)
	FLAGS_WARNING = -Wall -Wno-int-conversion -Wno-deprecated-declarations -Wextra
else
	FLAGS_WARNING = -Wall -Wextra
endif

all: $(NAME)

$(NAME):
	gcc $(SRC) $(FLAGS_WARNING) -lm -o $(NAME) -I$(INCLUDES)
	mv $(NAME) ../../

clean:
	$(RM)

fclean: clean
	$(RM) ../../$(NAME)

re:	fclean all

.PHONY: all clean fclean re
