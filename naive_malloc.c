#include "malloc.h"

void *find_unused(char *start_addr, size_t call_nb)
{
	while (call_nb > 0)
	{
		start_addr += *(size_t *)start_addr;
		--call_nb;
	}
	return (start_addr);
}

void *extend(size_t chunk_size, size_t *excess_mem)
{
	void *prev_brk;

	/* puts("sbrk called"); */
	prev_brk = sbrk(align(chunk_size, sysconf(_SC_PAGESIZE)));
	if (prev_brk == (void *) -1)
		return (NULL);
	*excess_mem += align(chunk_size, sysconf(_SC_PAGESIZE));
	return (prev_brk);
}

/**
 * malloc - naive version of malloc: dynamically allocates memory on the heap
 * @size: number of bytes to allocate
 *
 * Return: the memory address newly allocated, or NULL on error
 */
void *naive_malloc(size_t size)
{
	void *prev_brk;
	static void *start_addr;
	static size_t call_nb;
	size_t hdr_size, chunk_size, excess_mem;

	hdr_size = sizeof(size_t);
	chunk_size = align(size, sizeof(void *)) + hdr_size;
	/* printf("call_nb: %lu\n", call_nb); */
	/* printf("chunk_size: %lu\n", chunk_size); */
	/* printf("sbrk req: %lu\n", align(chunk_size, sysconf(_SC_PAGESIZE))); */
	if (!start_addr)
	{
		excess_mem = 0;
		start_addr = prev_brk = extend(chunk_size, &excess_mem);
		if (!prev_brk)
			return (NULL);
	}
	else
	{
		prev_brk = find_unused(start_addr, call_nb);
		excess_mem = *(size_t *)prev_brk;
		if (excess_mem < chunk_size + hdr_size)
			if (!extend(chunk_size, &excess_mem))
				return (NULL);
	}
	excess_mem -= chunk_size;
	/* printf("excess_mem: %lu\n", excess_mem); */
	*(size_t *)prev_brk = chunk_size;
	*(size_t *)((char *)prev_brk + *(size_t *)prev_brk) = excess_mem;
	++call_nb;
	return ((char *)prev_brk + hdr_size);
}

int main(void)
{
	char *str;
	int i;

	printf("Starting break is %p\n", sbrk(0));

	for (i = 0; i < 10; i++)
	{
		void *chunk;

		/* str = naive_malloc(10); */
		str = naive_malloc(1016);
		strcpy(str, "Holberton");
		str[21] = '\0';
		printf("%p: %s, ", (void *)str, str);
		chunk = (void *)(str - sizeof(size_t));
		printf("chunk addr: %p, ", (void *)chunk);
		printf("size: %lu, ", *((size_t *)chunk));
		printf("break: %p\n", sbrk(0));
	}

	printf("Final break is %p\n", sbrk(0));
	return (EXIT_SUCCESS);
}
