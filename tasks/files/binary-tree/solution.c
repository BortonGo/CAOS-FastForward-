#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct Node {
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

static int read_exact(int fd, void *buf, size_t count) {
    char *ptr = buf;
    size_t done = 0;

    while (done < count) {
        ssize_t rc = read(fd, ptr + done, count - done);
        if (rc < 0) {
            return -1;
        }
        if (rc == 0) {
            return -1;
        }
        done += (size_t)rc;
    }

    return 0;
}

static int read_node(int fd, int32_t idx, struct Node *node) {
    off_t offset = (off_t)idx * (off_t)sizeof(*node);

    if (lseek(fd, offset, SEEK_SET) == (off_t)-1) {
        return -1;
    }

    if (read_exact(fd, node, sizeof(*node)) < 0) {
        return -1;
    }

    return 0;
}

static int print_descending(int fd, int32_t idx) {
    if (idx == 0) {
        return 0;
    }

    struct Node node;
    if (read_node(fd, idx, &node) < 0) {
        return -1;
    }

    if (print_descending(fd, node.right_idx) < 0) {
        return -1;
    }

    printf("%" PRId32 " ", node.key);

    if (print_descending(fd, node.left_idx) < 0) {
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        return 1;
    }

    struct Node root;
    if (read_node(fd, 0, &root) < 0) {
        close(fd);
        return 1;
    }

    if (print_descending(fd, root.right_idx) < 0) {
        close(fd);
        return 1;
    }

    printf("%" PRId32 " ", root.key);

    if (print_descending(fd, root.left_idx) < 0) {
        close(fd);
        return 1;
    }

    printf("\n");

    close(fd);
    return 0;
}