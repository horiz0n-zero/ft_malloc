#include "libftmalloc.h"

static inline void		add_between(struct s_zone *const prev, struct s_zone *const zone, struct s_zone *const next)
{
	if (prev)
		prev->next = zone;
	if (next)
		next->prev = zone;
	zone->prev = prev;
	zone->next = next;
}

inline struct s_zone	*append_zone(struct s_malloc *const node, size_t size)
{
	struct s_zone		*zone;

	if (!(zone = request_zone(node, size)))
		return (NULL);
	if (node->pages->current > zone)
	{
		while (node->pages->current->prev && node->pages->current > zone)
			node->pages->current = node->pages->current->prev;
		add_between(node->pages->current->prev, zone, node->pages->current);
		return ((node->pages->current = zone));
	}
	else
	{
		while (node->pages->current->next && node->pages->current < zone)
			node->pages->current = node->pages->current->next;
		add_between(node->pages->current, zone, node->pages->current->next);
		return ((node->pages->current = zone));
	}
}
