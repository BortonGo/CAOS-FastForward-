#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include "read_file.h"

enum {
    BUF_SIZE = 4096,
};

void sha256b64buf(char *out, size_t out_size, const unsigned char *in, size_t in_size);

static int read_fail_count = -1;
static int malloc_fail_count = -1;

static inline size_t min(size_t a, size_t b) {
    return a < b ? a : b;
}

int main(int argc, char *argv[])
{
    if (argc >= 2) {
        sscanf(argv[1], "%d", &read_fail_count);
    }
    if (argc >= 3) {
        sscanf(argv[2], "%d", &malloc_fail_count);
    }
    char str[128];
    int newfd = dup(0);
    close(0);
    open("/dev/null", O_RDONLY, 0);
    struct FileContent fc = read_file(newfd);
    printf("%lld\n", (long long) fc.size);
    if (fc.data) {
        if (fc.size < 0) {
            printf("NONNULL\n");
        } else {
            sha256b64buf(str, sizeof(str), (const unsigned char*) fc.data, fc.size);
            printf("%s\n", str);
            printf("%02x\n", fc.data[fc.size]);
        }
        free(fc.data);
    } else {
        printf("NULL\n");
    }
}

ssize_t __real_read(int fd, void *ptr, size_t size);
ssize_t __wrap_read(int fd, void *ptr, size_t size)
{
    if (read_fail_count >= 0) {
        static int read_count = 0;
        if (read_count++ == read_fail_count) {
            errno = EIO;
            return -1;
        }
    }
    static int partial_read = -1;
    if (partial_read == -1) {
        char *val = getenv("PARTIAL_READ");
        partial_read = val && strtol(val, NULL, 10);
        if (partial_read) {
            srand(partial_read);
        }
    }
    if (partial_read && size) {
        return __real_read(fd, ptr, 1 + rand() % min(size, BUF_SIZE));
    }
    return __real_read(fd, ptr, size);
}

ssize_t __real___read_chk(int fd, void *buf, size_t count, size_t buflen);
ssize_t __wrap___read_chk(int fd, void *buf, size_t count, size_t buflen) {
    if (read_fail_count >= 0) {
        static int read_count = 0;
        if (read_count++ == read_fail_count) {
            errno = EIO;
            return -1;
        }
    }
    return __real___read_chk(fd, buf, count, buflen);
}

static int malloc_count = 0;

void *__real_malloc(size_t size);
void *__wrap_malloc(size_t size)
{
    if (malloc_fail_count >= 0) {
        if (malloc_count++ == malloc_fail_count) {
            errno = ENOMEM;
            return NULL;
        }
    }
    return __real_malloc(size);
}

void *__real_calloc(size_t s1, size_t s2);
void *__wrap_calloc(size_t s1, size_t s2)
{
    if (malloc_fail_count >= 0) {
        if (malloc_count++ == malloc_fail_count) {
            errno = ENOMEM;
            return NULL;
        }
    }
    return __real_calloc(s1, s2);
}

void *__real_realloc(void *p, size_t z);
void *__wrap_realloc(void *p, size_t z)
{
    if (malloc_fail_count >= 0) {
        if (malloc_count++ == malloc_fail_count) {
            errno = ENOMEM;
            return NULL;
        }
    }
    return __real_realloc(p, z);
}

void *__real_reallocarray(void *p, size_t nmemb, size_t size);
void *__wrap_reallocarray(void *p, size_t nmemb, size_t size)
{
    if (malloc_fail_count >= 0) {
        if (malloc_count++ == malloc_fail_count) {
            errno = ENOMEM;
            return NULL;
        }
    }
    return __real_reallocarray(p, nmemb, size);
}
