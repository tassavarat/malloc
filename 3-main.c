#include "malloc.h"
#include <stdio.h>

/**
 * pmem - print mem
 * @p: memory address to start printing from
 * @bytes: number of bytes to print
 *
 * Return: nothing
 */
void pmem(void *p, unsigned int bytes)
{
	unsigned char *ptr;
	unsigned int i;

	ptr = (unsigned char *)p;
	for (i = 0; i < bytes; i++)
	{
		if (i != 0)
		{
			printf(" ");
		}
		printf("%02x", *(ptr + i));
	}
	printf("\n");
}

int main()
{
	void *p;
	size_t nmemb = 20;
	size_t size = sizeof(size_t);

	setbuf(stdout, NULL);
	p = _calloc(nmemb, size);

	if (!p)
		return 1;
	pmem(p, nmemb * size);
	return 0;
}
