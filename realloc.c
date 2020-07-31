#include "malloc.h"

/**
 * _realloc - realloc() implementation
 * @ptr: pointer to the memory
 * @size: new size of the block of memory
 * Return: pointer to a new block of memory, NULL if failed
 */
void *_realloc(void *ptr, size_t size)
{
	char *s, *a;
	size_t i, limit, old_size;

	size = align_up(size, ALIGNMENT);
	old_size = ptr ? GET_SIZE((char *)ptr - HEADER_SIZE) : 0;
	if (old_size == size)
		return (ptr);
	if (ptr != NULL && size == 0)
	{
		_free(ptr);
		return (NULL);
	}
	if (ptr == NULL)
		return (_malloc(size));
	s = _malloc(sizeof(char) * size);
	if (!s)
		return (NULL);
	a = ptr;
	limit = size > old_size ? old_size : size;
	for (i = 0; i < limit; i++)
	{
		s[i] = a[i];
	}
	_free(ptr);
	return (s);
}
