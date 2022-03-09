NAME	=	malloc
SRCS	=	srcs/free.c \
			srcs/glob.c \
			srcs/main.c \
			srcs/malloc.c \
			srcs/realloc.c \
			srcs/show.c \
			srcs/utils.c
OBJS	=	$(SRCS:.c=.o)
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror
RM		=	rm -rf

all		:	$(NAME)

$(NAME)	:	$(OBJS)
			$(CC) $(CFLAGS) $^ -o $@

%.o	:	%.c
			$(CC) $(CFLAGS) -c $< -o $@

clean	:
			$(RM) $(OBJS)

fclean	:	clean
			$(RM) $(NAME)

re		:	fclean all