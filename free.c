#include "malloc.h"

/**
 * _free - free memory block
 * @ptr: pointer to a memory block
*/
void _free(void *ptr)
{
	char *p, *next;

	if (!ptr)
		return;
	p = ptr;
	p -= HDR_SZ;
	next = p + GET_SIZE(p);
	((block_info *)next)->prev = GET_SIZE(p);
	((block_info *)next)->size |= 1;
}
