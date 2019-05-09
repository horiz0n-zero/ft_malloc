/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_malloc.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 14:27:17 by afeuerst          #+#    #+#             */
/*   Updated: 2019/05/09 15:14:58 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_MALLOC_H
# define LIBFT_MALLOC_H

# include <sys/mman.h>
# include <unistd.h>

struct					s_libft_malloc
{
	size_t				pagesize;
	size_t				tiny_size;
	size_t				small_size;
};

# define TINY_SIZE 1
# define SMALL_SIZE 16

void					*malloc(
		size_t size) __attribute__((malloc));
void					*calloc(
		size_t count,
		size_t size) __attribute__((malloc));
void					*realloc(
		void *ptr,
		size_t size) __attribute__((malloc));
void					free(void *ptr);

#endif
