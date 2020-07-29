#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "malloc.h"

void *_malloc(size_t size);

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	char *str;
	int i;

	setbuf(stdout, NULL);
	printf("Starting break is %p\n", sbrk(0));

	for (i = 0; i < 10; i++)
	{
		void *chunk;

		str = _malloc(1024);
		strcpy(str, "Holberton");
		str[21] = '\0';
		printf("%p: %s, ", (void *)str, str);
		chunk = (void *)(str - sizeof(size_t));
		printf("chunk addr: %p, ", (void *)chunk);
		printf("size: %lu, ", *((size_t *)chunk));
		str = _malloc(278);
		strcpy(str, "Holberton");
		str[21] = '\0';
		printf("%p: %s, ", (void *)str, str);
		chunk = (void *)(str - sizeof(size_t));
		printf("chunk addr: %p, ", (void *)chunk);
		printf("size: %lu, ", *((size_t *)chunk));
		printf("break: %p\n", sbrk(0));
	}

	printf("Final break is %p\n", sbrk(0));
	print_heap();
	return (EXIT_SUCCESS);
}

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
/* int main(void) */
/* { */
/* 	char *str; */
/* 	int i; */

/* 	setbuf(stdout, NULL); */
/* 	printf("Starting break is %p\n", sbrk(0)); */

/* 	for (i = 0; i < 10; i++) */
/* 	{ */
/* 		void *chunk; */

/* 		str = naive_malloc_page(1016); */
/* 		strcpy(str, "Holberton"); */
/* 		str[21] = '\0'; */
/* 		printf("%p: %s, ", (void *)str, str); */
/* 		chunk = (void *)(str - sizeof(size_t)); */
/* 		printf("chunk addr: %p, ", (void *)chunk); */
/* 		printf("size: %lu, ", *((size_t *)chunk)); */
/* 		printf("break: %p\n", sbrk(0)); */
/* 	} */

/* 	printf("Final break is %p\n", sbrk(0)); */
/* 	return (EXIT_SUCCESS); */
/* } */
