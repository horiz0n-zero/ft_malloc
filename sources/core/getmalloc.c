#include "libftmalloc.h"

static struct s_malloc	g_malloc =
{
	~0,
	0,
	{
		NULL,
		0
	},
	{
		NULL,
		0
	},
	{
		NULL,
		0
	},
	NULL
}; // constructor

inline struct s_malloc	*getmalloc(void)
{
	if (g_malloc.init)
	{
		g_malloc.init = 0;
		g_malloc.pagesize = (size_t)getpagesize();
		g_malloc.tiny.pagesize = QUANTUM_TINY * g_malloc.pagesize;
		g_malloc.small.pagesize = QUANTUM_SMALL * g_malloc.pagesize;
	}
	return (&g_malloc);
}
