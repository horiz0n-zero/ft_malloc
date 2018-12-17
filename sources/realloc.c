#include "libftmalloc.h"

inline void					libftmalloc_memset(uint8_t *ptr, size_t size)
{
	while (size--)
		*ptr++ = 0;
}

inline void					libftmalloc_memmove(uint8_t *dst, const uint8_t *src, size_t size)
{
	while (size--)
		*dst++ = *src++;
}

static uint64_t				available_size(struct s_zone *const zone, union u_allocation *alloc)
{
	uint8_t					*mem;
	uint8_t *const			end_mem = (uint8_t*)zone + zone->size;
	uint64_t				total;

	total = (uint64_t)alloc->value.next;
	mem = (uint8_t*)(alloc + 1);
	while (mem < end_mem)
	{
		alloc = (union u_allocation*)(mem += alloc->value.next);
		mem += sizeof(union u_allocation);
		if (alloc->value.isfree)
			total += (uint64_t)alloc->value.next;
		else if (!alloc->value.next)
		{
			total += (end_mem - mem);
			break ;
		}
		else
			break ;
	}
	return (total);
}

static void					*try_realloc(struct s_malloc *const node, struct s_zone *const zone,
					union u_allocation *const alloc, const size_t size)
{
	uint8_t					*mem;
	uint64_t				nsize;
	void					*ptr;

	mem = (uint8_t*)(alloc + 1);
	while (((uint64_t)mem % ALIGNMENT))
		mem++;
	if (alloc->value.next >= (uint64_t)size)
		return (mem);
	if ((nsize = available_size(zone, alloc)) >= (uint64_t)size)
	{
		alloc->value.next = nsize;
		return (mem);
	}
	if ((ptr = malloc(size - (sizeof(union u_allocation) + ALIGNMENT))))
	{
		libftmalloc_memmove(ptr, mem, (size_t)(alloc->value.next - 16));
		alloc->value.isfree = 1;
	}
	return (ptr);
}

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
		if ((alloc = find_allocation_inzone(zone, ptr)) && !alloc->value.isfree)
			return (try_realloc(node, zone, alloc, size));
		else
			exit_realloc_unallocated(ptr);
	}
	else
		exit_realloc_unallocated(ptr);
	return (NULL);
}
