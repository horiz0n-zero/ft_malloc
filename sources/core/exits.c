#include "libftmalloc.h"

inline void			*enomem_error(void)
{
	errno = ENOMEM;
	return (NULL);
}

static void			print_ptr(uint64_t value, int lenght)
{
	char			printable[lenght + 1];
	size_t			count;

	write(STDERR_FILENO, "0x", 2);
	if (!lenght)
		return ((void)write(STDERR_FILENO, "0", 1));
	count = 0;
	while (value)
	{
		printable[--lenght] = "0123456789abcdef"[value % 16];
		value /= 16;
		count++;
	}
	write(STDERR_FILENO, printable, count);
}

inline void			exit_doublefree(void *const ptr)
{
	uint64_t		value;
	int				lenght;

	value = (uint64_t)ptr;
	lenght = 0;
	if (!ptr)
		lenght = 1;
	else
	{
		while (value)
			++lenght && (value /= 16);
	}
	write(STDERR_FILENO, "libftmalloc error: ", 19);
	print_ptr((uint64_t)ptr, lenght);
	write(STDERR_FILENO, " double free\n",
				sizeof(" double free\n"));
	exit(SIGABRT);
}

inline void			exit_realloc_unallocated(void *const ptr)
{
	uint64_t		value;
	int				lenght;

	value = (uint64_t)ptr;
	lenght = 0;
	if (!ptr)
		lenght = 1;
	else
	{
		while (value)
			++lenght && (value /= 16);
	}
	write(STDERR_FILENO, "libftmalloc error: ", 19);
	print_ptr((uint64_t)ptr, lenght);
	write(STDERR_FILENO, " pointer being realloc'd was not allocated\n",
				sizeof(" pointer being realloc'd was not allocated\n"));
	exit(SIGABRT);
}

inline void			exit_unallocated(void *const ptr)
{
	uint64_t		value;
	int				lenght;

	value = (uint64_t)ptr;
	lenght = 0;
	if (!ptr)
		lenght = 1;
	else
	{
		while (value)
			++lenght && (value /= 16);
	}
	write(STDERR_FILENO, "libftmalloc error: ", 19);
	print_ptr((uint64_t)ptr, lenght);
	write(STDERR_FILENO, " pointer being freed was not allocated\n",
				sizeof(" pointer being freed was not allocated\n"));
	exit(SIGABRT);
}
