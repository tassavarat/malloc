#include "malloc.h"

void *find_unused(char *start_addr, size_t call_nb, size_t *excess_mem)
{
	while (call_nb > 0)
	{
		*excess_mem -= *(size_t *)start_addr;
		if (*excess_mem == 0)
			*excess_mem = sysconf(_SC_PAGESIZE);
		start_addr += *(size_t *)start_addr;
		--call_nb;
	}
	return (start_addr);
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
	size_t excess_mem;

	/* printf("call_nb: %lu\n", call_nb); */
	/* printf("size req: %lu\n", size); */
	excess_mem = sysconf(_SC_PAGESIZE);
	if (!start_addr)
	{
		/* printf("alinged req: %lu\n", align(size)); */
		prev_brk = sbrk(sysconf(_SC_PAGESIZE));
		if (prev_brk == (void *) -1)
			return (NULL);
		start_addr = prev_brk;
	}
	else
	{
		prev_brk = find_unused(start_addr, call_nb, &excess_mem);
	}
	excess_mem -= size + sizeof(size_t);
	printf("excess_mem: %lu\n", excess_mem);
	/* if (excess_mem - size + sizeof(size_t) < size + sizeof(size_t)) */
	if (excess_mem < size + sizeof(size_t))
	{
		puts("CALLED");
		excess_mem = sysconf(_SC_PAGESIZE) - (size + sizeof(size_t));
		prev_brk = sbrk(sysconf(_SC_PAGESIZE));
		if (prev_brk == (void *) -1)
			return (NULL);
	}
	/* *(size_t *)prev_brk = align(size) + sizeof(size_t); */
	*(size_t *)prev_brk = size + sizeof(size_t);
	/* excess_mem -= *(size_t *)prev_brk; */
	printf("excess_mem: %lu\n", excess_mem);
	/* printf("target: %lu\n", *(size_t *)prev_brk); */
	*(size_t *)((char *)prev_brk + *(size_t *)prev_brk) = excess_mem;
	++call_nb;
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
