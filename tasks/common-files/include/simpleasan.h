#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

void *malloc_protected(size_t size) {
    //      page 0     |    pages 1..N   | page N + 1
    //  size | padding | [padding] | buf | inaccessible

    if (!size) {
        return NULL;
    }

    // Allocate.
    const size_t page_size = sysconf(_SC_PAGE_SIZE);
    size_t n_pages = (size - 1) / page_size + 1;
    void *buf = mmap(NULL, (n_pages + 2) * page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    assert(buf != MAP_FAILED);

    // Segfault on right buffer overflows/overreads.
    assert(!mprotect(buf + (n_pages + 1) * page_size, page_size, PROT_NONE));

    // Fill buffer and padding with junk.
    // Left buffer overreads will cause WA. Left overflows are not checked.
    memset(buf, 0x99, (n_pages + 1) * page_size);

    // Store size (for "free")
    memcpy(buf, &size, sizeof(size));

    // Actual buffer
    buf += (n_pages + 1) * page_size - size;
    return buf;
}

void free_protected(void *buf) {
    if (!buf) {
        return;
    }
    const size_t page_size = sysconf(_SC_PAGE_SIZE);

    // Find start of page containing buf.
    buf = (void *)((uintptr_t)buf & ~(page_size - 1));

    // Get size from the start of prev. page.
    buf -= page_size;
    size_t size;
    memcpy(&size, buf, sizeof(size));

    // Deallocate.
    size_t n_pages = (size - 1) / page_size + 1;
    assert(!munmap(buf, (n_pages + 2) * page_size));
}

