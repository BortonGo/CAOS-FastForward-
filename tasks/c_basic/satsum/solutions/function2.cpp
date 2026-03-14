
unsigned satsum(unsigned v1, unsigned v2)
{
    unsigned res;
    if (__builtin_uadd_overflow(v1, v2, &res)) {
        res = ~0;
    }
    return res;
}
