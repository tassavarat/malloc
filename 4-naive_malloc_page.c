#include "_malloc.h"

static heap_info heap;

void add_header(void *addr, size_t size, size_t prev)
{
	block_info *block;

	block = addr;
	block->size = prev ? size + HEADER_SIZE + 1 : size + HEADER_SIZE;
	block->prev = prev;
}

void *expand(size_t size)
{
	size_t page_size = PAGESIZE;
	size_t total_size, prev = 0;
	void *p;

	/*
	   Once we expand we need at least two headers,
	   one for the middle chunk and one for the header at the end
	   MIN_SIZE is for middle chunk, we don't want our middle chunk to have only header
	   We may need one more header when expand() is called for the firs time, otherwise
	   reuse sentinel chunk (chunk at the end of heap) by putting new size there
	*/
	total_size = 2 * HEADER_SIZE + MIN_SIZE + size;
	total_size += heap.heap_start ? 0 : HEADER_SIZE;
	for (; page_size < total_size; page_size += PAGESIZE)
		;
	p = sbrk(page_size);
	if (p == (void *)-1 && errno == ENOMEM)
		return (NULL);
	if (heap.heap_start == NULL)
	{
		/* Calling expand() for the first time */
		heap.heap_start = p;
		heap.heap_end = heap.heap_start + page_size;
	}
	else
	{
		/*
		  Subsequent calls to expand()
		  Make p point to sentinel chunk (block)
		 */
		p -= HEADER_SIZE;
		prev = GET_PREV(p);
		heap.heap_end += page_size;
	}
	/* Changing 3 headers */
	add_header(p, size, prev);
	add_header(p + size + HEADER_SIZE, page_size - size - 3 * HEADER_SIZE, 0);
	add_header(p + page_size - HEADER_SIZE, 0, page_size - size - 2 * HEADER_SIZE);

	return (p);
}

void *find_block(size_t size)
{
	void *p = heap.heap_start, *tmp;

	if (!p)
		return (NULL);
	while (GET_SIZE(p) >= 18)
	{
		p = p + GET_SIZE(p);

		if ((GET_SIZE(p) & 1) && GET_SIZE(p - GET_PREV(p)) >= size + HEADER_SIZE)
		{
			tmp = p;
			tmp -= GET_PREV(p);
			((block_info *)p)->size -= 1;
			((block_info *)p)->prev = 0;
			return (tmp);
		}
	}
	return (NULL);
}

void *_malloc(size_t size)
{
	size = align_up(size);
	void *p;

	p = find_block(size);
	if (!p)
		p = expand(size);
	if (!p)
		return (NULL);
	return (p + HEADER_SIZE);
}
