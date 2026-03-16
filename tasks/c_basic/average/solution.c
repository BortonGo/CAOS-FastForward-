#include "stdio.h"

int main() {
    unsigned a, b, c;

    if (scanf("%u %u", &a, &b) != 2) {
        return 1;
    }

    c = (a & b) + ((a ^ b) >> 1);

    if (printf("%u\n", c) < 0) {
        return 1;
    }
    return 0;
}
