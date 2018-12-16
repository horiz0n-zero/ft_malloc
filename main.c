#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void			show_alloc_mem(void);

int				main(void)
{
	void		*ptr;
	void		*s;

	s = malloc(100);
	ptr = malloc(42);
	free(ptr);
	free(s);
	show_alloc_mem();
}
