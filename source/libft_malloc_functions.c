/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_malloc_functions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 14:33:18 by afeuerst          #+#    #+#             */
/*   Updated: 2019/05/09 15:14:46 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_malloc_functions.h"

static struct s_libft_malloc	g_libft_malloc =
{
	0,
	0,
	0
};

void							libft_malloc_initializer(void)
{
	g_libft_malloc.pagesize = (size_t)getpagesize();
	g_libft_malloc.tiny_size = g_libft_malloc.pagesize * TINY_SIZE;
	g_libft_malloc.small_size = g_libft_malloc.pagesize * SMALL_SIZE;
}

void							*malloc(size_t size)
{
	return (NULL);
}

void							*calloc(size_t count, size_t size)
{
	return (NULL);
}

void							*realloc(void *ptr, size_t size)
{
	return (NULL);
}
