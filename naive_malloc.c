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
	*(size_t *)chunk = chunk_size;
	*(size_t *)(chunk + *(size_t *)chunk) = *excess_mem;
}

/**
 * find_unused - find unused chunk block
 * @heap_start: pointer to heap start
 * @call_nb: specifies how many times malloc has been called
 *
 * Return: pointer to start of unused chunk
 */
void *find_unused(char *heap_start, size_t call_nb)
{
	while (call_nb > 0)
	{
		heap_start += *(size_t *)heap_start;
		--call_nb;
	}
	return (heap_start);
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

	chunk = sbrk(align(chunk_size, sysconf(_SC_PAGESIZE)));
	if (chunk == (void *) -1)
		return (NULL);
	*excess_mem += align(chunk_size, sysconf(_SC_PAGESIZE));
	return (chunk);
}

/**
 * naive_malloc - naive malloc: dynamically allocates memory to the heap
 * @size: number of bytes to allocate
 *
 * Return: the memory address newly allocated, or NULL on error
 */
void *naive_malloc(size_t size)
{
	void *chunk;
	static void *heap_start;
	static size_t call_nb;
	size_t hdr_size, chunk_size, excess_mem;

	hdr_size = sizeof(size_t);
	chunk_size = align(size, sizeof(void *)) + hdr_size;
	if (!heap_start)
	{
		excess_mem = 0;
		heap_start = chunk = extend(chunk_size, &excess_mem);
		if (!chunk)
			return (NULL);
	}
	else
	{
		chunk = find_unused(heap_start, call_nb);
		excess_mem = *(size_t *)chunk;
		if (excess_mem < chunk_size + hdr_size)
			if (!extend(chunk_size, &excess_mem))
				return (NULL);
	}
	set_hdr(chunk, chunk_size, &excess_mem);
	++call_nb;
	return ((char *)chunk + hdr_size);
}
