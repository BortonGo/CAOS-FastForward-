#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "read_file.h"

enum { BUF_SIZE = 4096 };

struct FileContent read_file(int fd) {
    struct FileContent ret = {-1, NULL};
    char buffer[BUF_SIZE];

    size_t capacity = 0;
    size_t size = 0;
    char *data = NULL;

    while (1) {
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer));

        if (bytes_read < 0) {
            free(data);
            return ret;
        }

        if (bytes_read == 0) {
            break;
        }

        if (size + (size_t)bytes_read + 1 > capacity) {
            size_t new_capacity;

            if (capacity == 0) {
                new_capacity = BUF_SIZE;
            } else {
                new_capacity = capacity * 2;
            }

            while (new_capacity < size + (size_t)bytes_read + 1) {
                new_capacity *= 2;
            }

            char *new_data = realloc(data, new_capacity);
            if (new_data == NULL) {
                free(data);
                return ret;
            }

            data = new_data;
            capacity = new_capacity;
        }

        memcpy(data + size, buffer, (size_t)bytes_read);
        size += (size_t)bytes_read;
    }

    if (data == NULL) {
        data = malloc(1);
        if (data == NULL) {
            return ret;
        }
    }

    data[size] = '\0';

    ret.size = (ssize_t)size;
    ret.data = data;
    return ret;
}