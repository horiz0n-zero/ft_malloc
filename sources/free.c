#include "libftmalloc.h"

inline union u_allocation		*find_allocation_inzone(struct s_zone *const zone, void *const ptr)
{
	union u_allocation			*alloc;
	uint8_t						*mem;
	uint8_t						*tmp;
	uint8_t *const				end_page = (uint8_t*)zone + zone->size;

	alloc = (union u_allocation*)(zone + 1);
	mem = (uint8_t*)alloc + sizeof(union u_allocation);
	while (mem < end_page && alloc->value.next)
	{
		tmp = mem;
		while (((uint64_t)mem % ALIGNMENT))
			mem++;
		if ((void*)mem == ptr)
			return (alloc);
		mem = tmp + alloc->value.next;
		alloc = (union u_allocation*)mem;
		mem = (uint8_t*)alloc + sizeof(union u_allocation);
	}
	return (NULL);
}

inline struct s_zone			*find_zone(struct s_malloc *const node, void *const ptr)
{
	struct s_zone				*zone;

	zone = node->pages->current;
	if (ptr > (void*)zone)
	{
		while (zone)
		{
			if (ptr < (void*)((uint8_t*)zone + zone->size))
				return (zone);
			zone = zone->next;
		}
	}
	zone = node->pages->current->prev;
	while (zone)
	{
		if (ptr > (void*)zone)
			return (zone);
		zone = zone->prev;
	}
	return (NULL);
}

inline void						remove_zone(struct s_malloc *const node, struct s_zone *const zone)
{
	struct s_zone				*prev;
	struct s_zone				*next;

	prev = node->pages->current->prev;
	next = node->pages->current->next;
	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;
	if (node->pages->current == zone)
	{
		if (next)
			node->pages->current = next;
		else
			node->pages->current = prev;
	}
	munmap(zone, zone->size);
}

static void						zone_try_remove(struct s_malloc *const node, struct s_zone *const zone)
{
	union u_allocation			*alloc;
	uint8_t *const				end_mem = (uint8_t*)zone + zone->size;
	uint8_t						*mem;

	alloc = (union u_allocation*)(zone + 1);
	mem = (uint8_t*)(alloc + 1);
	while (mem < end_mem)
	{
		if (!alloc->value.next)
			break ;
		else if (!alloc->value.isfree)
			return ;
		alloc = (union u_allocation*)(mem += alloc->value.next);
		mem = (uint8_t*)(alloc + 1);
	}
	remove_zone(node, zone);
}

void							free(void *ptr)
{
	struct s_malloc *const		node = getmalloc();
	struct s_zone				*zone;
	union u_allocation			*alloc;

	if (ptr)
	{
		if ((zone = find_zone(node, ptr)))
		{
			if ((alloc = find_allocation_inzone(zone, ptr)))
			{
				if (alloc->value.isfree)
					exit_doublefree(ptr);
				alloc->value.isfree = 1;
				zone_try_remove(node, zone);
			}
			else
				exit_unallocated(ptr);
		}
		else
			exit_unallocated(ptr);
	}
}
