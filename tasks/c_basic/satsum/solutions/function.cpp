
unsigned satsum(unsigned v1, unsigned v2)
{
    unsigned res = v1 + v2;
    if (res < v1 || res < v2) res = ~0;
    return res;
}
