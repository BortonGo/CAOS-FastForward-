#include <stdlib.h>

void *gc_malloc(size_t size, finalizer_t finalizer) {
    void *result = malloc(size);
    ...
    return result;
}

void gc_collect_impl(uintptr_t stack_top) {
    ...
}

void gc_init(char **argv) {
     // stack bottom <-> (uintptr_t)argv - 3;
}
