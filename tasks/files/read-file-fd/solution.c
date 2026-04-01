#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "read_file.h"

enum {
    BUF_SIZE = 4096,
    MIN_CAPACITY = 16,
};

struct FileContent read_file(int fd) {
    struct FileContent ret = {-1L, NULL};
    size_t capacity = 0;
    size_t size = 0;
}
