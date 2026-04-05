#include "gc.h"

#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct allocation allocation_t;

struct allocation {
    void *ptr;
    size_t size;
    finalizer_t finalizer;
    bool alive;
    allocation_t *next;
};

static allocation_t *g_allocations = NULL;
static uintptr_t g_stack_bottom = 0;

static bool points_to(void *ptr, allocation_t *a) {
    uintptr_t uptr = (uintptr_t)ptr;
    uintptr_t aptr = (uintptr_t)a->ptr;
    return (uptr >= aptr) && (uptr - aptr <= a->size);
}

static uintptr_t align_up_uintptr(uintptr_t value, size_t alignment) {
    uintptr_t mask = (uintptr_t)alignment - 1;
    return (value + mask) & ~mask;
}

static void mark_ptr(void *candidate) {
    for (allocation_t *a = g_allocations; a != NULL; a = a->next) {
        if (points_to(candidate, a)) {
            a->alive = true;
            return;
        }
    }
}

static void scan_region(uintptr_t begin, uintptr_t end) {
    if (begin > end) {
        uintptr_t tmp = begin;
        begin = end;
        end = tmp;
    }

    const size_t ptr_align = alignof(void *);
    const size_t ptr_size = sizeof(void *);

    uintptr_t p = align_up_uintptr(begin, ptr_align);

    while (p + ptr_size <= end) {
        void *candidate = *(void **)p;
        mark_ptr(candidate);
        p += ptr_align;
    }
}

void gc_init(char **argv) {
    g_stack_bottom = (uintptr_t)argv;
}

void *gc_malloc(size_t size, finalizer_t finalizer) {
    void *mem = malloc(size);
    if (mem == NULL) {
        return NULL;
    }

    allocation_t *node = (allocation_t *)malloc(sizeof(allocation_t));
    if (node == NULL) {
        free(mem);
        return NULL;
    }

    node->ptr = mem;
    node->size = size;
    node->finalizer = finalizer;
    node->alive = false;
    node->next = g_allocations;
    g_allocations = node;

    return mem;
}

void gc_collect_impl(uintptr_t stack_top) {
    for (allocation_t *a = g_allocations; a != NULL; a = a->next) {
        a->alive = false;
    }

    scan_region(stack_top, g_stack_bottom);

    bool changed;
    do {
        changed = false;

        for (allocation_t *a = g_allocations; a != NULL; a = a->next) {
            if (!a->alive) {
                continue;
            }

            size_t old_count = 0;
            for (allocation_t *t = g_allocations; t != NULL; t = t->next) {
                if (t->alive) {
                    ++old_count;
                }
            }

            scan_region((uintptr_t)a->ptr, (uintptr_t)a->ptr + a->size);

            size_t new_count = 0;
            for (allocation_t *t = g_allocations; t != NULL; t = t->next) {
                if (t->alive) {
                    ++new_count;
                }
            }

            if (new_count != old_count) {
                changed = true;
            }
        }
    } while (changed);

    allocation_t **link = &g_allocations;
    while (*link != NULL) {
        allocation_t *cur = *link;

        if (!cur->alive) {
            if (cur->finalizer != NULL) {
                cur->finalizer(cur->ptr, cur->size);
            }
            free(cur->ptr);
            *link = cur->next;
            free(cur);
        } else {
            link = &cur->next;
        }
    }
}