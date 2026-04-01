#include <stdio.h>

void copy_file(int in_fd, int out_fd);

int main(int argc, char *argv[]) {
    copy_file(0, 1);

    return 0;
}
