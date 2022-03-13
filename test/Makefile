NAME	=	test
SRCS	=	test.c
OBJS	=	$(SRCS:.c=.o)
CC	=	gcc
CFLAGS	=	-Wall -Wextra
RM	=	rm -rf

all	:	$(NAME)

$(NAME)	:	$(OBJS)
		cd .. && $(MAKE)
		$(CC) $(CFLAGS) $^ -o $@ -L.. -Wl,-rpath=.. -lft_malloc

%.o	:	%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean	:
		cd .. && $(MAKE) clean
		$(RM) $(OBJS)

fclean	:	clean
		cd .. && $(MAKE) fclean
		$(RM) $(NAME)

re	:	fclean all
