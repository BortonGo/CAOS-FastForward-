#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

extern struct Block *freelist;

struct Block {
    uint64_t size;
    struct Block *next;
};

static size_t align8(size_t size) {
    return (size + (size_t)7) & ~(size_t)7;
}

void *alloc_mem(size_t size) {
    struct Block *prev = NULL;
    struct Block *cur = freelist;
    struct Block *block;
    size_t actual_size;
    size_t total_size;

    while (cur != NULL) {
        if (cur->size >= size) {
            if (prev == NULL) {
                freelist = cur->next;
            } else {
                prev->next = cur->next;
            }
            return (char *)cur + sizeof(cur->size);
        }
        prev = cur;
        cur = cur->next;
    }

    if (size > SIZE_MAX - (size_t)7) {
        return NULL;
    }

    actual_size = align8(size);
    if (actual_size < 8) {
        actual_size = 8;
    }

    if (actual_size > SIZE_MAX - sizeof(block->size)) {
        return NULL;
    }

    total_size = sizeof(block->size) + actual_size;
    if (total_size > (size_t)INTPTR_MAX) {
        return NULL;
    }

    block = (struct Block *)sbrk((intptr_t)total_size);
    if (block == (void *)-1) {
        return NULL;
    }

    block->size = actual_size;
    return (char *)block + sizeof(block->size);
}

void free_mem(void *ptr) {
    struct Block *block;

    if (ptr == NULL) {
        return;
    }

    block = (struct Block *)((char *)ptr - sizeof(block->size));
    block->next = freelist;
    freelist = block;
}