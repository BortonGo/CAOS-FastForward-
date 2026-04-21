#include <sys/types.h>
#include <unistd.h>

ssize_t copy_file(int in_fd, int out_fd) {
    char buffer[4096];
    ssize_t total = 0;

    while (1) {
        ssize_t bytes_read = read(in_fd, buffer, sizeof(buffer));
        if (bytes_read == 0) {
            return total;  // EOF
        }
        if (bytes_read < 0) {
            return -1;
        }

        ssize_t written_total = 0;
        while (written_total < bytes_read) {
            ssize_t bytes_written = write(out_fd, buffer + written_total,
                                          bytes_read - written_total);

            if (bytes_written < 0) {
                return -1;
            }

            written_total += bytes_written;
        }

        total += bytes_read;
    }
}