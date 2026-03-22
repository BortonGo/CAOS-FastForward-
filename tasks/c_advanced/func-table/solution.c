#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef double (*funcptr_t)(double);

struct FunctionTable {
    const char *name;
    funcptr_t func;
};

...

int main(void) {
    ...
}
