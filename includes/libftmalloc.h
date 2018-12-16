#ifndef LIBFTMALLOC_H
# define LIBFTMALLOC_H

# include <sys/mman.h>
# include <unistd.h>
# include <stddef.h>
# include <stdint.h>
# include <signal.h>
# include <sys/errno.h>

void						*malloc(size_t size);
void						*calloc(size_t count, size_t size);
void						*realloc(void *ptr, size_t size);
void						free(void *ptr);

# define QUANTUM_TINY 16
# define QUANTUM_SMALL 512

# define ALIGNMENT sizeof(uint64_t)
# define PROT PROT_READ | PROT_WRITE
# define FLAGS MAP_ANON | MAP_PRIVATE

typedef struct s_malloc		t_malloc;
typedef struct s_zone		t_zone;
typedef union u_allocation	t_allocation;

struct						s_pages
{
	struct s_zone			*current;
	size_t					pagesize;
};

struct						s_malloc
{
	uint64_t				init; // constructor attribute norme ???
	size_t					pagesize;
	struct s_pages			tiny;
	struct s_pages			small;
	struct s_pages			large;
	struct s_pages			*pages; // current size
};
extern struct s_malloc		*getmalloc(void);

struct						s_zone // size 16
{
	struct s_zone			*prev; // will be : b = a ^ c
	struct s_zone			*next;
	size_t					size;
};

union						u_allocation
{
	struct					s_allocation // attribute size 8
	{
		unsigned int		isfree:1;
		uint64_t			next:63;
	}						value;
	uint64_t				raw;
};

// constructor init
/*
 **
 */

extern struct s_zone		*request_zone(struct s_malloc *const node, size_t size);

void						*search_memory(struct s_malloc *const node, size_t size);
void						*zone_search(struct s_zone *const zone, size_t size);

extern struct s_zone		*append_zone(struct s_malloc *const node, size_t size);

extern struct s_zone		*find_zone(struct s_malloc *const node, void *const ptr);
extern union u_allocation	*find_allocation_inzone(struct s_zone *const zone, void *const ptr);

extern void					remove_zone(struct s_malloc *const node, struct s_zone *const zone);

void						exit(int signal);
extern void					exit_unallocated(void *const ptr);
extern void					exit_doublefree(void *const ptr);
extern void					exit_realloc_unallocated(void *const ptr);
extern void					*enomem_error(void);

extern void					libftmalloc_memset(uint8_t *ptr, size_t size);

void						show_alloc_mem(void);

#endif




