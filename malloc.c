#include "malloc.h"

static heap_info heap;

/**
 * print_heap - prints contents of heap, used for visualization
*/
void print_heap(void)
{
	char *p = heap.heap_start;
	size_t i = 0;

	if (!p)
		return;
	while (GET_SIZE(p) >= 18)
	{
		printf("%sChunk # %lu, %sAddr: %p\n%s", MAG, i, BLU, (void *)p, reset);
		printf("\t%sPREV: %lu, %sSIZE: %lu%s\n", GRN, GET_PREV(p),
		       RED, GET_SIZE(p), reset);
		p += GET_SIZE(p) & 1 ? GET_SIZE(p) - 1 : GET_SIZE(p);
		++i;
	}
	printf("%sChunk # %lu, %sAddr: %p\n%s", MAG, i, BLU, (void *)p, reset);
	printf("\t%sPREV: %lu, %sSIZE: %lu%s\n", GRN, GET_PREV(p),
	       RED, GET_SIZE(p), reset);

	if (p + HEADER_SIZE != sbrk(0))
		printf("%s\t\tChunk end and Heap end do not match!!!%s\n", BRED, reset);
	else
		printf("%s\t\tChunk end is good!!!%s\n", BGRN, reset);
}

/**
 * add_header - add header info
 * @addr: header address
 * @size: size of this memory block, size + HEADER_SIZE
 * @prev: previous chunk offset, 0 if previous is allocated
*/
void add_header(void *addr, size_t size, size_t prev)
{
	block_info *block;

	block = addr;
	block->size = prev ? size + HEADER_SIZE + 1 : size + HEADER_SIZE;
	block->prev = prev;
}

/**
 * expand - expand heap by calling sbrk()
 * @size: size needed by user
 * Return: pointer to memory block header
*/
void *expand(size_t size)
{
	size_t page_size;
	char *p, *tmp;

	/**
	 * Once we expand we need at least two headers,
	 * one for the middle chunk and one for the header at the end
	 * MIN_SIZE is for middle chunk, we don't want our middle
	 * chunk to have only header. We may need one more header when expand()
	 * is called for the firs time, otherwise
	 * reuse sentinel chunk (chunk at the end of heap) by putting new size there
	 */
	page_size = 2 * HEADER_SIZE + MIN_SIZE + size;
	page_size += heap.heap_start ? 0 : HEADER_SIZE;
	page_size = align_up(page_size, PAGESIZE);
	p = sbrk(page_size);
	if (p == (void *)-1 && errno == ENOMEM)
		return (NULL);
	if (heap.heap_start == NULL)
	{
		/* Calling expand() for the first time */
		heap.heap_start = p;

		/* Set the first chunk, returned to USER */
		add_header(p, size, 0);
		/* Set middle and sentinel chunks */
		add_header(p + size + HEADER_SIZE,
			   page_size - size - 3 * HEADER_SIZE, 0);
		add_header(p + page_size - HEADER_SIZE, 0,
			   page_size - size - 2 * HEADER_SIZE);
		return (p);
	}

	else
	{
		/*
		 * Subsequent calls to expand()
		 * Make p point to sentinel chunk (block) and convert
		 * that sentinel to regular chunk, which will be returned to
		 * a user
		 */
		p -= HEADER_SIZE;
		tmp = p;
		/* Change previous sentinel chunk, returned to USER */
		add_header(p, size, GET_PREV(p));
		p += GET_SIZE(p) & 1 ? GET_SIZE(p) - 1 : GET_SIZE(p);
		add_header(p, page_size - size - 2 * HEADER_SIZE, 0);
		add_header(p + GET_SIZE(p), 0, page_size - size - HEADER_SIZE);
		return (tmp);
	}
}

/**
 * find_block - find an unused memory block to return to a user
 * @size: size needed by a user
 * Return: pointer to a memory block header
*/
void *find_block(size_t size)
{
	char *p = heap.heap_start, *tmp;
	size_t required_size = size + HEADER_SIZE;

	if (!p)
		return (NULL);

	while (GET_SIZE(p) >= HEADER_SIZE + MIN_SIZE)
	{
		p += GET_SIZE(p) & 1 ? GET_SIZE(p) - 1 : GET_SIZE(p);
		if ((GET_SIZE(p) & 1) && GET_SIZE(p - GET_PREV(p)) >= required_size)
		{
			tmp = p - GET_PREV(p);
			if (GET_SIZE(tmp) >= required_size + HEADER_SIZE + MIN_SIZE)
			{
				/* Split the chunk */

				/* middle = tmp + required_size; */
				/* ((block_info *)middle)->size = GET_SIZE(tmp) - required_size; */
				/* ((block_info *)middle)->prev = 0; */
				add_header(tmp + required_size,
					   GET_SIZE(tmp) - required_size - HEADER_SIZE, 0);
				((block_info *)tmp)->size = required_size;
				((block_info *)p)->prev = GET_SIZE(tmp + required_size);
			}
			else
			{
				/* Don't split the chunk */
				/* ((block_info *)p)->size -= 1; */
				/* ((block_info *)p)->prev = 0; */
				add_header(p, GET_SIZE(p) - 1, 0);
			}

			return (tmp);
		}
	}
	return (NULL);
}

/**
 * _malloc - malloc function implementation
 * @size: size need by a user
 * Return: a pointer to heap with at least @size usable memory,
 * NULL on failure or when @size is 0
 */
void *_malloc(size_t size)
{
	char *p;

	size = align_up(size, ALIGNMENT);

	if (size == 0)
		return (NULL);
	p = find_block(size);
	if (!p)
		p = expand(size);
	if (!p)
		return (NULL);
	return (p + HEADER_SIZE);
}
