#include "malloc.h"
#include <string.h>


int main(void)
{
	char *p;
	size_t len;

	p = _malloc(32);
	strcpy(p, "Hello");
	printf("%s\n", p);
	printf("Addr: %p\n", (void *)p);
	p = _realloc(p, 128);
	len = strlen(p);
	strcpy(p + len, " My name is Bobby. I like to earn money by doing nothing!!!\n");
	printf("%s\n", p);
	printf("Addr: %p\n", (void *)p);
	_free(p);
	return 0;
}
