
#include <stdint.h>

uint32_t satsum(uint32_t v1, uint32_t v2)
{
    uint32_t res = v1 + v2;
    if (res < v2) res = ~0;
    return res;
}
