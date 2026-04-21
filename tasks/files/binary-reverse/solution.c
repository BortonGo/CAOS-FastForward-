#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum { RECORD_SIZE = 10 };

struct Data {
    int16_t x;
    int64_t y;
};

static void marshall(unsigned char *out, const struct Data *in) {
    uint16_t ux;
    uint64_t uy;

    if (in->x >= 0) {
        ux = (uint16_t)in->x;
    } else {
        ux = (uint16_t)((int32_t)in->x + 0x10000);
    }

    if (in->y >= 0) {
        uy = (uint64_t)in->y;
    } else if (in->y == INT64_MIN) {
        uy = (1ULL << 63);
    } else {
        uint64_t mag = (uint64_t)(-in->y);
        uy = (~mag) + 1ULL;
    }

    out[0] = (unsigned char)(ux & 0xffu);
    out[1] = (unsigned char)((ux >> 8) & 0xffu);

    out[2] = (unsigned char)(uy & 0xffu);
    out[3] = (unsigned char)((uy >> 8) & 0xffu);
    out[4] = (unsigned char)((uy >> 16) & 0xffu);
    out[5] = (unsigned char)((uy >> 24) & 0xffu);
    out[6] = (unsigned char)((uy >> 32) & 0xffu);
    out[7] = (unsigned char)((uy >> 40) & 0xffu);
    out[8] = (unsigned char)((uy >> 48) & 0xffu);
    out[9] = (unsigned char)((uy >> 56) & 0xffu);
}

static void unmarshall(struct Data *out, const unsigned char *in) {
    uint16_t ux = 0;
    uint64_t uy = 0;

    ux |= (uint16_t)in[0];
    ux |= (uint16_t)in[1] << 8;

    uy |= (uint64_t)in[2];
    uy |= (uint64_t)in[3] << 8;
    uy |= (uint64_t)in[4] << 16;
    uy |= (uint64_t)in[5] << 24;
    uy |= (uint64_t)in[6] << 32;
    uy |= (uint64_t)in[7] << 40;
    uy |= (uint64_t)in[8] << 48;
    uy |= (uint64_t)in[9] << 56;

    if ((ux & 0x8000u) == 0) {
        out->x = (int16_t)ux;
    } else {
        out->x = (int16_t)((int32_t)ux - 0x10000);
    }

    if ((uy & (1ULL << 63)) == 0) {
        out->y = (int64_t)uy;
    } else if (uy == (1ULL << 63)) {
        out->y = INT64_MIN;
    } else {
        uint64_t mag = (~uy) + 1ULL;
        out->y = -(int64_t)mag;
    }
}

static void die_io(void) {
    fprintf(stderr, "error\n");
    exit(2);
}

static void die_overflow(void) {
    fprintf(stderr, "overflow\n");
    exit(3);
}

static void pread_exact(int fd, unsigned char *buf, size_t count,
                        off_t offset) {
    size_t done = 0;

    while (done < count) {
        ssize_t rc = pread(fd, buf + done, count - done, offset + (off_t)done);
        if (rc < 0) {
            die_io();
        }
        if (rc == 0) {
            die_io();
        }
        done += (size_t)rc;
    }
}

static void pwrite_exact(int fd, const unsigned char *buf, size_t count,
                         off_t offset) {
    size_t done = 0;

    while (done < count) {
        ssize_t rc = pwrite(fd, buf + done, count - done, offset + (off_t)done);
        if (rc < 0) {
            die_io();
        }
        if (rc == 0) {
            die_io();
        }
        done += (size_t)rc;
    }
}

static void transform(struct Data *d, int32_t a) {
    int64_t delta = (int64_t)d->x * (int64_t)a;

    if (delta > 0 && d->y > INT64_MAX - delta) {
        die_overflow();
    }
    if (delta < 0 && d->y < INT64_MIN - delta) {
        die_overflow();
    }

    d->y += delta;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        return 1;
    }

    char *endptr = NULL;
    errno = 0;
    long parsed = strtol(argv[2], &endptr, 10);
    if (errno != 0 || endptr == argv[2] || *endptr != '\0' ||
        parsed < INT32_MIN || parsed > INT32_MAX) {
        return 1;
    }
    int32_t a = (int32_t)parsed;

    int fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        die_io();
    }

    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size < 0) {
        close(fd);
        die_io();
    }

    if (file_size % RECORD_SIZE != 0) {
        close(fd);
        die_io();
    }

    off_t count = file_size / RECORD_SIZE;

    unsigned char raw_left[RECORD_SIZE];
    unsigned char raw_right[RECORD_SIZE];
    struct Data left;
    struct Data right;

    for (off_t i = 0; i < count / 2; ++i) {
        off_t j = count - 1 - i;
        off_t off_i = i * RECORD_SIZE;
        off_t off_j = j * RECORD_SIZE;

        pread_exact(fd, raw_left, sizeof(raw_left), off_i);
        pread_exact(fd, raw_right, sizeof(raw_right), off_j);

        unmarshall(&left, raw_left);
        unmarshall(&right, raw_right);

        transform(&left, a);
        transform(&right, a);

        marshall(raw_left, &left);
        marshall(raw_right, &right);

        pwrite_exact(fd, raw_right, sizeof(raw_right), off_i);
        pwrite_exact(fd, raw_left, sizeof(raw_left), off_j);
    }

    if (count % 2 != 0) {
        off_t mid = count / 2;
        off_t off_mid = mid * RECORD_SIZE;

        pread_exact(fd, raw_left, sizeof(raw_left), off_mid);
        unmarshall(&left, raw_left);
        transform(&left, a);
        marshall(raw_left, &left);
        pwrite_exact(fd, raw_left, sizeof(raw_left), off_mid);
    }

    if (close(fd) < 0) {
        die_io();
    }

    return 0;
}