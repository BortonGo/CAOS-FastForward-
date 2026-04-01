#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int rand_initialized;

ssize_t __real_read(int fd, void *ptr, size_t size);
ssize_t __real_write(int fd, const void *ptr, size_t size);

ssize_t __wrap_read(int fd, void *ptr, size_t size)
{
    //__real_write(2, "R", 1);
    if (!rand_initialized) {
        srand(time(0));
        rand_initialized = 1;
    }
    if (size > 4096) size = 4096;
    if (size > 0) {
        size_t newsize = rand() % size;
        if (newsize > 0) size = newsize;
    }
    return __real_read(fd, ptr, size);
}

ssize_t __wrap_write(int fd, const void *ptr, size_t size)
{
    //__real_write(2, "W", 1);
    if (!rand_initialized) {
        srand(time(0));
        rand_initialized = 1;
    }
    if (size > 4096) size = 4096;
    if (size > 0) {
        size_t newsize = rand() % size;
        if (newsize > 0) size = newsize;
    }
    return __real_write(fd, ptr, size);
}
