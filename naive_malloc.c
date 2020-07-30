#include "malloc.h"

/**
 * set_hdr - set chunk headers
 * @chunk: pointer to start of chunk
 * @chunk_size: size of chunk
 * @excess_mem: pointer to variable tracking excess memory
 */
void set_hdr(char *chunk, size_t chunk_size, size_t *excess_mem)
{
	*excess_mem -= chunk_size;
	/* printf("excess_mem: %lu\n", *excess_mem); */
	*(size_t *)chunk = chunk_size;
	*(size_t *)(chunk + *(size_t *)chunk) = *excess_mem;
}

/**
 * find_unused - find unused chunk block
 * @start_addr: pointer to heap start
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
	void *chunk;

	/* puts("sbrk called"); */
	chunk = sbrk(align(chunk_size, sysconf(_SC_PAGESIZE)));
	if (chunk == (void *) -1)
		return (NULL);
	*excess_mem += align(chunk_size, sysconf(_SC_PAGESIZE));
	return (chunk);
}

/**
 * naive_malloc - naive malloc: dynamically allocates memory on the heap
 * @size: number of bytes to allocate
 *
 * Return: the memory address newly allocated, or NULL on error
 */
void *naive_malloc(size_t size)
{
	void *chunk;
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
		start_addr = chunk = extend(chunk_size, &excess_mem);
		if (!chunk)
			return (NULL);
	}
	else
	{
		chunk = find_unused(start_addr, call_nb);
		excess_mem = *(size_t *)chunk;
		if (excess_mem < chunk_size + hdr_size)
			if (!extend(chunk_size, &excess_mem))
				return (NULL);
	}
	set_hdr(chunk, chunk_size, &excess_mem);
	++call_nb;
	return ((char *)chunk + hdr_size);
}
