#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

#define LSB(x, n) ((x) & ((1 << (n)) - 1))
#define TEST_BIT(x, n) ((x) & (1 << (n)))

enum {
    MIN_CYRILLIC_CODEPOINT = 0x400,
    MAX_CYRILLIC_CODEPOINT = 0x4ff,
};

int main(void) {
    int ch;
    int codepoints = 0;
    int cyrillic = 0;

    while ((ch = getchar()) != EOF) {
        unsigned char b0 = (unsigned char)ch;
        unsigned int cp = 0;

        if ((b0 & 0x80) == 0) {
            cp = b0;
        } else if ((b0 & 0xE0) == 0xC0) {
            int ch1 = getchar();
            assert(ch1 != EOF);
            unsigned char b1 = (unsigned char)ch1;

            cp = ((unsigned int)(b0 & 0x1F) << 6) | ((unsigned int)(b1 & 0x3F));
        } else if ((b0 & 0xF0) == 0xE0) {
            int ch1 = getchar();
            int ch2 = getchar();
            assert(ch1 != EOF && ch2 != EOF);
            unsigned char b1 = (unsigned char)ch1;
            unsigned char b2 = (unsigned char)ch2;

            cp = ((unsigned int)(b0 & 0x0F) << 12) |
                 ((unsigned int)(b1 & 0x3F) << 6) | ((unsigned int)(b2 & 0x3F));
        } else {
            int ch1 = getchar();
            int ch2 = getchar();
            int ch3 = getchar();
            assert(ch1 != EOF && ch2 != EOF && ch3 != EOF);
            unsigned char b1 = (unsigned char)ch1;
            unsigned char b2 = (unsigned char)ch2;
            unsigned char b3 = (unsigned char)ch3;

            cp = ((unsigned int)(b0 & 0x07) << 18) |
                 ((unsigned int)(b1 & 0x3F) << 12) |
                 ((unsigned int)(b2 & 0x3F) << 6) | ((unsigned int)(b3 & 0x3F));
        }

        ++codepoints;

        if (cp >= MIN_CYRILLIC_CODEPOINT && cp <= MAX_CYRILLIC_CODEPOINT) {
            ++cyrillic;
        }
    }

    printf("%d %d\n", codepoints, cyrillic);
    return 0;
}
