#include "libftmalloc.h"

static int			number_size(uint64_t n, const uint64_t base)
{
	int				c;

	c = 0;
	while (n)
	{
		c++;
		n /= base;
	}
	return (c);
}

static void			print_number(uint64_t n, uint64_t base, const char *const prefix)
{
	const int		count = number_size(n, base);
	char			buffer[count + 1];
	int				i;

	i = count - 1;
	if (prefix)
		write(STDOUT_FILENO, prefix, 2);
	if (!n)
		return ((void)write(STDOUT_FILENO, "0", 1));
	while (n)
	{
		buffer[i--] = "0123456789ABCDEF"[n % base];
		n /= base;
	}
	write(STDOUT_FILENO, buffer, (size_t)count);
}

static size_t		print_allocs(struct s_zone *const zone)
{
	uint8_t *const		end = (uint8_t*)zone + zone->size;
	union u_allocation	*alloc;
	uint8_t				*mem;
	size_t				r;

	alloc = (union u_allocation*)(zone + 1);
	mem = (uint8_t*)(alloc + 1);
	r = 0;
	while (mem < end)
	{
		if (!alloc->value.next)
			break ;
		else if (!alloc->value.isfree)
		{
			print_number((uint64_t)mem, 16, "0x");
			write(STDOUT_FILENO, " - ", 3);
			print_number((uint64_t)(mem + alloc->value.next), 16, "0x");
			write(STDOUT_FILENO, " : ", 3);
			print_number((uint64_t)(alloc->value.next - 16), 10, NULL);
			write(STDOUT_FILENO, " octets\n", sizeof(" octets\n"));
			r += (size_t)(alloc->value.next - 16);
		}
		alloc = (union u_allocation*)(mem += alloc->value.next);
		mem += sizeof(union u_allocation);
	}
	return (r);
}

static size_t		show_pages(struct s_pages *const page, const char *const str, const size_t ssize)
{
	struct s_zone	*ptr;
	size_t			size;

	if (!(ptr = page->current))
	{
		write(STDOUT_FILENO, str, ssize);
		write(STDOUT_FILENO, "NULL\n", 5);
		return (0);
	}
	while (ptr && ptr->prev)
		ptr = ptr->prev;
	size = 0;
	while (ptr)
	{
		write(STDOUT_FILENO, str, ssize);
		print_number((uint64_t)ptr, 16, "0x");
		write(STDOUT_FILENO, "\n", 1);
		size += print_allocs(ptr);
		ptr = ptr->next;
	}
	return (size);
}

void				show_alloc_mem(void)
{
	struct s_malloc	*node = getmalloc();
	size_t			size;

	size = show_pages(&node->tiny, "TINY : ", 7);
	size += show_pages(&node->small, "SMALL : ", 8);
	size += show_pages(&node->large, "LARGE : ", 8);
	write(STDOUT_FILENO, "Total : ", 8);
	print_number((uint64_t)size, 10, NULL);
	write(STDOUT_FILENO, " octets\n", 8);
}
