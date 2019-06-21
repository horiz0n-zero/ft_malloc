# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/05/09 14:12:37 by afeuerst          #+#    #+#              #
#    Updated: 2019/06/10 11:16:44 by afeuerst         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so

SOURCE = $(shell find ./source -type f | grep "\.c")
INCLUDE = ./include/

FLAGS = -shared -O0 -g -init _libft_malloc_initializer -lpthread

all: $(NAME)

$(NAME): $(SOURCE) $(INCLUDE) Makefile
	clang $(FLAGS) $(SOURCE) -I $(INCLUDE) -o $(NAME)
	ln -fs $(NAME) $(LINK)

clean:

fclean:
	rm -rf $(NAME) $(NAME).dSYM $(LINK)

re: fclean all
