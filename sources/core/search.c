#include "libftmalloc.h"

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
		{
			alloc->value.next = (uint64_t)size;
			if ((end_zone - (mem + size)) >= size)
			{
				alloc = (union u_allocation*)(mem + size);
				alloc->raw = 0;
			}
			while (((uint64_t)mem % ALIGNMENT))
				mem++;
			return (mem);
		}
		else if (alloc->value.isfree && alloc->value.next <= size)
		{
			alloc->value.next = (uint64_t)size;
			alloc->value.isfree = 0;
			alloc = (union u_allocation*)(mem + size);
			alloc->raw = 0;
			while (((uint64_t)mem % ALIGNMENT))
				mem++;
			return (mem);
		}
		else
		{
			alloc = (union u_allocation*)(mem += alloc->value.next);
			mem += sizeof(union u_allocation); // optionel ??
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
