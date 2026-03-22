#include "gc.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

void shout(void *ptr, size_t size) {
    printf("finalizer called: %zu\n", size);
}

struct alloc {
    void* addr;
    size_t size;
};

int main(int argc, char **argv) {
    gc_init(argv);
    size_t nallocs;
    if (scanf("%zu", &nallocs) != 1) {
        return 1;
    }
    struct alloc allocs[nallocs];

    while (1) {
        int cmd = 0, allocno, allocno2;
        size_t size, idx;
        ssize_t offset;
        void* addr;
        uintptr_t* obj;
        while (1) {
            int c = getchar();
            if (c < 0 || c == '\n')
                break;
        }
        if (scanf("%d", &cmd) == EOF) {
            return 0;
        }
        switch (cmd) {
            case 1: // new allocation
                assert(scanf("%d %zu", &allocno, &size) == 2);
                addr = gc_malloc(size, shout);
                if (!addr) {
                    fprintf(stderr, "Failed to allocate %d\n", allocno);
                    return 1;
                }
                allocs[allocno].addr = addr;
                allocs[allocno].size = size;
                memset(addr, 0, size);
                printf("Allocated object %d of size %zu\n", allocno, size);
                break;
            case 2: // remove ref from stack
                assert(scanf("%d", &allocno) == 1);
                allocs[allocno].addr = 0;
                printf("Deleted stack ref to %d\n", allocno);
                break;
            case 3: // add ref
                assert(scanf("%d %zu %d %zd", &allocno, &idx, &allocno2, &offset) == 4);
                if (offset == -1) {
                    offset = allocs[allocno2].size;
                }
                obj = allocs[allocno].addr;
                obj[idx] = (uintptr_t) ((void*) allocs[allocno2].addr + offset);
                printf("Set ref #%zu in %d to %d + %zd\n", idx, allocno, allocno2, offset);
                break;
            case 4: // delete ref
                assert(scanf("%d %zu", &allocno, &idx) == 2);
                obj = allocs[allocno].addr;
                obj[idx] = 0;
                printf("Unset ref #%zu in %d\n", idx, allocno);
                break;
            case 5: // call GC
                addr = 0;  // reset temporary refs
                obj = 0;
                printf("Call GC\n");
                gc_collect();
                break;
        }
    }
}
