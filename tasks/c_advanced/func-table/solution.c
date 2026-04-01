#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef double (*funcptr_t)(double);

struct FunctionTable {
    const char *name;
    funcptr_t func;
};

static const struct FunctionTable function_table[] = {
    {"sin", sin}, {"cos", cos},   {"exp", exp}, {"log", log},
    {"tan", tan}, {"sqrt", sqrt}, {NULL, NULL},
};

static char *read_token(void) {
    int c;

    do {
        c = getchar();
        if (c == EOF) {
            return NULL;
        }
    } while (isspace((unsigned char)c));

    size_t cap = 16;
    size_t len = 0;
    char *s = malloc(cap);
    if (s == NULL) {
        exit(1);
    }

    while (c != EOF && !isspace((unsigned char)c)) {
        if (len + 1 >= cap) {
            cap *= 2;
            char *tmp = realloc(s, cap);
            if (tmp == NULL) {
                free(s);
                exit(1);
            }
            s = tmp;
        }

        s[len++] = (char)c;
        c = getchar();
    }

    s[len] = '\0';
    return s;
}

int main(void) {
    while (1) {
        char *name = read_token();
        if (name == NULL) {
            break;
        }

        double x;
        if (scanf("%lf", &x) != 1) {
            free(name);
            break;
        }

        const struct FunctionTable *p = function_table;

        while (p->name != NULL) {
            if (strcmp(name, p->name) == 0) {
                printf("%a\n", p->func(x));
                break;
            }
            ++p;
        }

        if (p->name == NULL) {
            printf("%a\n", NAN);
        }

        free(name);
    }

    return 0;
}