ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so

SOURCES = $(shell find ./sources -name "*.c")
INCLUDES = -I ./includes
FLAGS = -g -O0

OBJS = $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(SOURCES) $(OBJ_DIR) $(OBJS) main.c
	clang $(FLAGS) $(INCLUDES) $(OBJS) -shared -o $(NAME)
	clang -O0 -g main.c -L. -lft_malloc_x86_64_Darwin

%.o: %.c
	clang $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)
	rm -rf a.out.dSYM
	rm -rf a.out

re: fclean all
