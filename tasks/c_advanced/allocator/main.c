#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

void* alloc_mem(size_t size);

void free_mem(void* ptr);

struct Block {
    uint64_t size;
    struct Block* next;
};

struct Block* freelist = 0;

#define debug(fmt, ...) do { if (verbose) { fprintf(stderr, fmt, __VA_ARGS__); } } while(0)

int main() {
    size_t nallocs, verbose;
    if (scanf("%zu%zu", &nallocs, &verbose) != 2) {
        return 1;
    }
    void* addresses[nallocs];
    int counter = 0;

    while (1) {
        char size_buf[64];
        int cmd = 0, allocno, allocno2;
        size_t size;
        void* addr;
        while (1) {
            int c = getchar();
            if (c < 0 || c == '\n')
                break;
        }
        counter += 1;
        if (scanf("%d", &cmd) != 1) {
            printf("%d\n", counter);
            return 0;
        }
        switch (cmd) {
            case 1: // new allocation
                if (scanf("%d%63s", &allocno, size_buf) != 2) {
                    fprintf(stderr, "Failed to scan %d\n", __LINE__);
                    return 1;
                }
                size = strtoull(size_buf, NULL, 0);
                addresses[allocno] = addr = alloc_mem(size);
                debug("allocation #%d: %p\n", allocno, addr);
                if (addr) {
                    memset(addr, 0xf1, size);
                }
                break;
            case 2: // free
                if (scanf("%d", &allocno) != 1) {
                    fprintf(stderr, "Failed to scan %d\n", __LINE__);
                    return 1;
                }
                addr = addresses[allocno];
                debug("freeing allocation #%d: %p\n", allocno, addr);
                free_mem(addr);
                break;
            case 3: // check that two addresses are the same
                if (scanf("%d%d", &allocno, &allocno2) != 2) {
                    fprintf(stderr, "Failed to scan %d\n", __LINE__);
                    return 1;
                }
                debug("checking equivalence of #%d and #%d\n", allocno, allocno2);
                assert(addresses[allocno] == addresses[allocno2]);
                break;
            case 4: // check that an allocation succeeded or failed
                if (scanf("%d%zu", &allocno, &size) != 2) {
                    fprintf(stderr, "Failed to scan %d\n", __LINE__);
                    return 1;
                }
                addr = addresses[allocno];
                debug("checking successfullness %zu of #%d (%p)\n", size, allocno, addr);
                if (size) {
                    assert(addresses[allocno] != 0);
                } else {
                    assert(addresses[allocno] == 0);
                }
                break;
            case 5: // new allocation no fill
                if (scanf("%d%63s", &allocno, size_buf) != 2) {
                    fprintf(stderr, "Failed to scan %d\n", __LINE__);
                    return 1;
                }
                size = strtoull(size_buf, NULL, 0);
                addresses[allocno] = addr = alloc_mem(size);
                debug("allocation #%d: %p\n", allocno, addr);
                break;
            default:
                fprintf(stderr, "Unknown command %d\n", cmd);
                return 1;
        }
    }
}
