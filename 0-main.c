#include "malloc.h"

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
