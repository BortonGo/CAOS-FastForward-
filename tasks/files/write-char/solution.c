#include <sys/syscall.h>
#include <sys/types.h>

enum { BUFFER_SIZE = 4096 };

struct FileWriteState {
    int fd;
    unsigned char *buf;
    ssize_t bufsize;
    ssize_t index;
};

static unsigned char stout_buffer[BUFFER_SIZE];

static struct FileWriteState stout_struct = {1, stout_buffer, BUFFER_SIZE, 0};

struct FileWriteState *stout = &stout_struct;

void flush(struct FileWriteState *out) {
    if (out->index == 0) {
        return;
    }

    long ret;
    asm volatile("syscall"
                 : "=a"(ret)
                 : "a"(SYS_write), "D"(out->fd), "S"(out->buf), "d"(out->index)
                 : "rcx", "r11", "memory");

    out->index = 0;
}

void writechar(int c, struct FileWriteState *out) {
    out->buf[out->index] = (unsigned char)c;
    ++out->index;

    if (out->index == out->bufsize) {
        flush(out);
    }
}