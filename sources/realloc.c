#include "libftmalloc.h"

inline void					libftmalloc_memset(uint8_t *ptr, size_t size)
{
	while (size--)
		*ptr++ = 0;
}
/*
static void					*try_realloc(struct s_malloc *const node, struct s_zone *const zone,
								union u_allocation *const alloc, const size_t size)
{
	uint8_t *const			end_page = (uint8_t*)zone + zone->size;
	uint8_t					*mem;
	union u_allocation		*next;
	size_t					lenght;

	mem = ((uint8_t*)(alloc + 1)) + alloc->value.size;
	lenght = alloc->value.size;
	while (mem < end_page)
	{
		next = (union u_allocation*)mem;
		if (next->value.isfree)
			lenght += next->value.size;
		else if (!next->value.size)
			lenght += (end_zone - mem);
	}
}*/

void						*realloc(void *ptr, size_t size)
{
	struct s_malloc *const	node = getmalloc();
	struct s_zone			*zone;
	union u_allocation		*alloc;

	if (!ptr)
		return (malloc(size));
	else if (!size)
		free(ptr);
	else if ((zone = find_zone(node, ptr)))
	{
		size += (sizeof(union u_allocation) + ALIGNMENT);
		if ((alloc = find_allocation_inzone(zone, ptr)))
			return (ptr); //return (try_realloc(node, zone, alloc));
		else
			exit_realloc_unallocated(ptr);
	}
	else
		exit_realloc_unallocated(ptr);
	return (NULL);
}
