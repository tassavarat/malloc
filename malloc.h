#ifndef MALLOC_H
#define MALLOC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define align(size) ((size + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1))

#endif /* MALLOC_H */
