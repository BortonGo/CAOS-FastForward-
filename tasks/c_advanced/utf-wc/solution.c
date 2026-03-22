#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

#define LSB(x, n) (x & ((1 << (n)) - 1))
#define TEST_BIT(x, n) (x & (1 << (n)))

enum {
    MIN_CYRILLIC_CODEPOINT = 0x400,
    MAX_CYRILLIC_CODEPOINT = 0x4ff,
};

int main() {
    // TODO your code here
}
