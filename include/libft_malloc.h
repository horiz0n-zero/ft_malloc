/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_malloc.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 14:27:17 by afeuerst          #+#    #+#             */
/*   Updated: 2019/06/10 11:37:51 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_MALLOC_H
# define LIBFT_MALLOC_H

# include <sys/mman.h>
# include <unistd.h>
# include <pthread.h>

# define TINY_SIZE   1
# define SMALL_SIZE  16
# define TINY_MASK   (size_t)0xfff
# define SMALL_MASK ((size_t)0xffff & ~TINY_MASK)
# define LARGE_MASK ((size_t)~0 & ~(SMALL_MASK | TINY_MASK))

# define ALIGN sizeof(uint64_t)

typedef struct s_control_zone	t_control_zone;
typedef struct s_zone			t_zone;
typedef struct s_alloc			t_alloc;
typedef struct s_marker			t_marker;

struct							s_marker
{
	unsigned int				flags:8;
	size_t						size:56;
# define IS_MARKER    1 << 0
# define MARKER_ZONE  1 << 1
# define MARKER_LARGE 1 << 2
# define MARKER_SMALL 1 << 2
# define MARKER_FREE  1 << 3
	/*
	** if IS_MARKER { zone or alloc }
	** if MARKER_ZONE { is zone } :
	** if MARKER_LARGE { size };
	** if !MARKER_ZONE { is alloc } :
	** if MARKER_FREE { is free };
	** if MARKER_SMALL { is small else tiny };
	*/
} __attribute__((packed,aligned(ALIGN)));

struct							s_alloc
{
	struct s_marker				marker;
} __attribute__((packed,aligned(ALIGN)));

struct							s_zone
{
	struct s_marker				marker;
} __attribute__((packed,aligned(ALIGN)));

struct							s_control_zone
{
	struct s_control_zone		*prev;
	struct s_control_zone		*next;
} __attribute__((packed,aligned(ALIGN)));

struct							s_libft_malloc
{
	size_t						pagesize;
	size_t						tiny_size;
	size_t						small_size;

	pthread_mutex_t				mutex;

	struct s_control_zone		*control_zone;
} __attribute__((packed,aligned(ALIGN)));

void							*malloc(
		size_t size) __attribute__((malloc));
void							*calloc(
		size_t count,
		size_t size) __attribute__((malloc));
void							*realloc(
		void *ptr,
		size_t size) __attribute__((malloc));
void							free(void *ptr);

#endif
