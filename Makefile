ifeq ($(HOSTTYPE),)
HOSTTYPE	?=	$(shell uname -m)_$(shell uname -s)
endif

NAME		=	libft_malloc_$(HOSTTYPE).so
SYML		=	libft_malloc.so
SRCS		=	srcs/free.c \
			srcs/glob.c \
			srcs/malloc.c \
			srcs/realloc.c \
			srcs/show.c \
			srcs/utils.c
OBJS		=	$(SRCS:.c=.o)
CC		=	gcc
EFLAGS		=	-Wall -Wextra -Werror
CFLAGS		=	-fPIC
LFLAGS		=	-shared
LIBS		=	-lpthread
RM		=	rm -rf

all		:	$(NAME)

$(NAME)		:	$(OBJS)
			$(CC) $^ $(EFLAGS) $(LFLAGS) -o $@ $(LIBS)
			$(RM) $(SYML)
			ln -s $(NAME) $(SYML)
			$(info "gcc main.c -L. -Wl,-rpath=. -lft_malloc" to compile with main file)

%.o		:	%.c
			$(CC) $(EFLAGS) $(CFLAGS) -c $< -o $@

clean		:
			$(RM) $(OBJS)

fclean		:	clean
			$(RM) $(NAME) $(SYML)

re		:	fclean all
