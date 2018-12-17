#include "libftmalloc.h"

inline struct s_zone			*request_zone(struct s_malloc *const node, size_t size)
{
	struct s_zone				*zone;
	union u_allocation			*alloc;

	if (!node->pages->pagesize)
	{
		while ((size % node->pagesize))
			size++; // - % ?
	}
	else
		size = node->pages->pagesize;
	if (!(zone = mmap(0, size, PROT, FLAGS, -1, 0)))
		return (enomem_error());
	zone->prev = NULL;
	zone->next = NULL;
	zone->size = size;
	alloc = (union u_allocation*)(zone + 1);
	alloc->raw = 0;
	return (zone);
}

static inline void				*search_zone(struct s_malloc *const node, size_t size)
{
	struct s_zone				*zone;
	void						*ptr;

	if (!node->pages->current)
	{
		if (!(zone = request_zone(node, size)))
			return (NULL);
		node->pages->current = zone;
		return (zone_search(zone, size));
	}
	else
	{
		if (!(ptr = search_memory(node, size)))
			if ((zone = append_zone(node, size)))
				ptr = zone_search(zone, size);
	}
	return (ptr);
}

void							*malloc(size_t size)
{
	struct s_malloc *const		node = getmalloc();

	if (!size)
		return (NULL);
	size += (sizeof(union u_allocation) + ALIGNMENT);
	if (size <= node->tiny.pagesize)
		node->pages = (void*)(((uint8_t*)node) + offsetof(struct s_malloc, tiny));
	else if (size <= node->small.pagesize)
		node->pages = (void*)(((uint8_t*)node) + offsetof(struct s_malloc, small));
	else
		node->pages = (void*)(((uint8_t*)node) + offsetof(struct s_malloc, large));
	return (search_zone(node, size));
}

void							*calloc(size_t count, size_t size)
{
	size_t						total;
	void						*ptr;

	if (!count || !size)
		return (NULL);
	total = count * size;
	if ((ptr = malloc(total)))
		libftmalloc_memset(ptr, total);
	return (ptr);
}
