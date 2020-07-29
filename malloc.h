#ifndef MALLOC_H
#define MALLOC_H

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "ANSI-color-codes.h"

#define ALIGNMENT sizeof(void *)
#define MIN_SIZE sizeof(void *)
#define PAGESIZE sysconf(_SC_PAGESIZE)
#define HEADER_SIZE sizeof(block_info)
#define align_up(num, align) (((num) + ((align) - 1)) & ~((align) - 1))

/* Macros to get struct members */
#define LSB_ZERO_MASK 0xfffffffffffffffe
/* Get Size and Prev */
#define _GET_SIZE(p) (((block_info *)(p))->size)
#define _GET_PREV(p) (((block_info *)(p))->prev)

/* Get Size and Prev with unset LSB*/
#define GET_SIZE(p) ((((block_info *)(p))->size) & LSB_ZERO_MASK)
#define GET_PREV(p) ((((block_info *)(p))->prev) & LSB_ZERO_MASK)

/**
 * struct heap_info_s - stores info about heap
 * @heap_start: start of heap
 * @heap_end: end of heap
*/
typedef struct heap_info_s
{
	void *heap_start;
	void *heap_end;
} heap_info;

/**
 * struct block_info_s - stores info block
 * @prev: prev
 * @size: size
*/
typedef struct block_info_s
{
	size_t prev;
	size_t size;
} block_info;

void print_heap(void);
void *naive_malloc(size_t size);
void *_malloc(size_t size);
void _free(void *ptr);
void *_calloc(size_t nmemb, size_t size);

void *malloc(size_t size);
void free(void *ptr);
void *_realloc(void *ptr, size_t size);

#endif /* MALLOC_H */
