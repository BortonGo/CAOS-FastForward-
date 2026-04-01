#include <sys/syscall.h>
#include <sys/types.h>

extern void *stout;

void flush(void *f);

void writechar(int c, void *f);

ssize_t read(int fd, char *buf, ssize_t size) {
    ssize_t res = 0;
    asm volatile("syscall"
                 : "=a"(res)
                 : "a"(SYS_read), "D"(fd), "S"(buf), "d"(size));
    return res;
}

void _start() {
    char buf[1024];
    ssize_t cur = 0;
    ssize_t size = 0;

    while (1) {
        if (cur >= size) {
            size = read(0, buf, sizeof(buf));
            if (size <= 0) {
                break;
            }
            cur = 0;
        }

        writechar(buf[cur], stout);
        ++cur;
    }
    flush(stout);
    flush(stout);

    asm volatile("syscall" : : "a"(SYS_exit), "D"(0));
}