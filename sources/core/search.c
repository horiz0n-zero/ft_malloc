#include "libftmalloc.h"

static inline void		*set_alloc(union u_allocation *alloc, uint8_t *mem, uint8_t *const end, const size_t size)
{
	if (!alloc->value.isfree)
	{
		alloc->value.next = (uint64_t)size;
		if ((mem + size + sizeof(union u_allocation)) < end)
		{
			alloc = (union u_allocation*)(mem + size);
			alloc->raw = 0;
		}
	}
	else
		alloc->value.isfree = 0;
	while (((uint64_t)mem % ALIGNMENT))
		mem++;
	return (mem);
}

void					*zone_search(struct s_zone *const zone, size_t size)
{
	union u_allocation	*alloc;
	uint8_t *const		end_zone = (((uint8_t*)zone) + zone->size);
	uint8_t				*mem;

	alloc = (union u_allocation*)(zone + 1);
	mem = (uint8_t*)(alloc + 1);
	while (mem < end_zone)
	{
		if (!alloc->value.next && (end_zone - mem) >= size)
			return (set_alloc(alloc, mem, end_zone, size));
		else if (alloc->value.isfree && alloc->value.next <= size)
			return (set_alloc(alloc, mem, end_zone, size));
		else
		{
			alloc = (union u_allocation*)(mem += alloc->value.next);
			mem += sizeof(union u_allocation);
		}
	}
	return (NULL);
}

void					*search_memory(struct s_malloc *const node, size_t size)
{
	struct s_zone		*zone;
	void				*ptr;

	if ((ptr = zone_search(node->pages->current, size)))
		return (ptr);
	zone = node->pages->current->next;
	while (zone)
	{
		if ((ptr = zone_search(zone, size)))
		{
			node->pages->current = zone;
			return (ptr);
		}
		zone = zone->next;
	}
	zone = node->pages->current->prev;
	while (zone)
	{
		if ((ptr = zone_search(zone, size)))
		{
			node->pages->current = zone;
			return (ptr);
		}
		zone = zone->prev;
	}
	return (NULL);
}
