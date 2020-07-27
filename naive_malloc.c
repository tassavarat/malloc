#include "malloc.h"

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

	if (!start_addr)
	{
		printf("page_size: %li\n", sysconf(_SC_PAGESIZE));
		printf("size req: %lu\n", size);
		printf("alinged req: %lu\n", align(size));
		prev_brk = sbrk(sysconf(_SC_PAGESIZE));
		if (prev_brk == (void *) -1)
			return (NULL);
		start_addr = prev_brk;
		printf("start_addr: %p\n", start_addr);
	}
	++call_nb;
	printf("call_nb: %lu\n", call_nb);
	*(size_t *)prev_brk = align(size) + sizeof(size_t);
	return ((char *)prev_brk + sizeof(size_t));
}

int main(void)
{
	char *str;
	int i;

	printf("Starting break is %p\n", sbrk(0));

	for (i = 0; i < 10; i++)
	{
		void *chunk;

		str = naive_malloc(10);
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
