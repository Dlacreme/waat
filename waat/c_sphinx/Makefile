##
## Makefile
##
## Made by Mathieu Delacroix
## mathieu.delacroix.fr@gmail.com
##
## Started on  CN Th07 09 13:16:57 2017 Creme
## Last update CN Th07 09 13:17:02 2017 Creme
##

SRC     =	mic_reader.c	\
			mic_process.c

NAME    =	mic_reader

FLAG    = -DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\"	`pkg-config --cflags --libs pocketsphinx sphinxbase`
#FLAG   += -g
#FLAG   += -O2 -s

CFLAGS  = $(FLAG)

LDFLAGS = $(FLAG)

OBJ     = $(SRC:.c=.o)

RM      = rm -f

CC      = gcc

ARGS	= 

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(OBJ) $(LDFLAGS) -o $(NAME) 

clean:
		$(RM) $(OBJ) *.swp *~ *#

fclean: clean
		$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
