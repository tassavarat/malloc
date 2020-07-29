#ifndef MALLOC_H
#define MALLOC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define align(offset, align) ((offset + (align - 1)) & - align)

#endif /* MALLOC_H */
