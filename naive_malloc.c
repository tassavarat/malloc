#include "malloc.h"

/**
 * find_unused - find unused chunk block
 * @start_addr: start address (head of the heap)
 * @call_nb: specifies how many times malloc has been called
 *
 * Return: pointer to start of unused chunk
 */
void *find_unused(char *start_addr, size_t call_nb)
{
	while (call_nb > 0)
	{
		start_addr += *(size_t *)start_addr;
		--call_nb;
	}
	return (start_addr);
}

/**
 * extend - calls sbrk with size aligned to page size
 * @chunk_size: aligned size request
 * @excess_mem: pointer to variable tracking excess memory
 *
 * Return: pointer to start of new chunk, NULL on failure
 */
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
 * naive_malloc - naive version of malloc: dynamically allocates
 * memory on the heap
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
