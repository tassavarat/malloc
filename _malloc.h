#ifndef _MALLOC_H
#define _MALLOC_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "ANSI-color-codes.h"

#define ALIGNMENT sizeof(void *)
#define MIN_SIZE sizeof(void *)
#define PAGESIZE sysconf(_SC_PAGESIZE)
#define HEADER_SIZE sizeof(block_info)
#define align_up(num) (((num) + ((ALIGNMENT) - 1)) & ~((ALIGNMENT) - 1))
#define align_pagesize(num) (((num) + ((PAGESIZE) - 1)) & ~((PAGESIZE) - 1))

/* Macros to get struct members */
#define GET_SIZE(p) (((block_info *)(p))->size)
#define GET_PREV(p) (((block_info *)(p))->prev)

typedef struct heap_info_s
{
	void *heap_start;
	void *heap_end;
} heap_info;

typedef struct block_info_s
{
	size_t prev;
	size_t size;
} block_info;

static heap_info heap;
void print_heap(void);
#endif /* _MALLOC_H */
