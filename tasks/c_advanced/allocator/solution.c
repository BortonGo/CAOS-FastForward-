#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

extern struct Block *freelist;

struct Block {
    uint64_t size;
    struct Block *next;
};

void *alloc_mem(size_t size) {
    // TODO your code here
}

void free_mem(void *ptr) {
    // TODO your code here
}
