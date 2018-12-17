#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void			show_alloc_mem(void);

int				main(void)
{
	uint32_t	count = 12096;
	uint32_t	r;
	void		*ptr;

	while (count--)
	{
		r = arc4random_uniform(1000);
		ptr = malloc((size_t)r);
		if (!arc4random_uniform(3))
			free(ptr);
		else
		{
			r = arc4random_uniform(1000);
			ptr = realloc(ptr, (size_t)r);
		}
	}
	show_alloc_mem();
}
