#include <sys/syscall.h>
#include <sys/types.h>

enum { BUFFER_SIZE = 4096 };

struct FileWriteState {
    int fd;  // "файловый дескриптор", для вывода на стандартный поток вывода - 1
    unsigned char *buf;  // указатель на буфер
    ssize_t bufsize;     // размер буфера
    ssize_t index;
};

static unsigned char buf[BUFFER_SIZE];
static struct FileWriteState stout_struct = ...;

struct FileWriteState *stout = &stout_struct;
