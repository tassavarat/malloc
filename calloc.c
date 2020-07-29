#include "malloc.h"

/**
 * _calloc - allocates memory which is initialized to 0
 * @nmemb: number of elements
 * @size: size of each element
 * Return: allocates memory for an array of nmemb elements of
 * size bytes each and returns a pointer to the
 *  allocated memory.  The memory is set to zero
*/
void *_calloc(size_t nmemb, size_t size)
{
	size_t required_size;
	void *p;

	/* TODO: handle overflow */
	required_size = nmemb * size;
	p = _malloc(required_size);
	if (!p)
		return (NULL);
	memset(p, 0, required_size);
	return (p);
}
