/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_malloc_functions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 14:33:18 by afeuerst          #+#    #+#             */
/*   Updated: 2019/06/10 11:36:42 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_malloc_functions.h"

static struct s_libft_malloc	g_libft_malloc =
{
	0,
	0,
	0,
	PTHREAD_MUTEX_INITIALIZER,

	NULL
};

void							libft_malloc_initializer(void)
{
	g_libft_malloc.pagesize = (size_t)getpagesize();
	g_libft_malloc.tiny_size = g_libft_malloc.pagesize * TINY_SIZE;
	g_libft_malloc.small_size = g_libft_malloc.pagesize * SMALL_SIZE;
}

void							*malloc(size_t size)
{
	if (size)
	{
		pthread_mutex_lock(&g_libft_malloc.mutex);
		if (size & LARGE_MASK)
			;
		else if (size & SMALL_MASK)
			;
		else
			;
		pthread_mutex_unlock(&g_libft_malloc.mutex);
	}
	return (NULL);
}

void							*calloc(size_t count, size_t size)
{
	const size_t				total_size = count * size;
	register uint64_t *const	start_ptr = malloc(total_size);

	if (start_ptr)
	{
		size = total_size / ALIGN;
		if (total_size % ALIGN)
			++size;
		while (size > 0)
			start_ptr[--size] = 0;
	}
	return (start_ptr);
}

void							*realloc(void *ptr, size_t size)
{
	if (!ptr)
		return (malloc(size));
	else if (!size)
		free(ptr);
	else
	{
		pthread_mutex_lock(&g_libft_malloc.mutex);
		;
		pthread_mutex_unlock(&g_libft_malloc.mutex);
	}
	return (NULL);
}

void							free(void *ptr)
{
	if (ptr)
	{
		pthread_mutex_lock(&g_libft_malloc.mutex);
		;
		pthread_mutex_unlock(&g_libft_malloc.mutex);
	}
}
