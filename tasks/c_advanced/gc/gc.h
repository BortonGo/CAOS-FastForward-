#ifndef CAOS_GC_H
#define CAOS_GC_H

#include <stdlib.h>

typedef void (*finalizer_t)(void *ptr, size_t size);

void gc_init(char **argv);
void *gc_malloc(size_t size, finalizer_t finalizer);
void gc_collect();

#endif
