#include <stdio.h>
#include <stdint.h>

uint32_t satsum(uint32_t v1, uint32_t v2);

int main(void) {
    unsigned v1, v2, res;
    while (scanf("%u%u", &v1, &v2) == 2) {
        res = satsum(v1, v2);
        printf("%u\n", res);
    }
}
