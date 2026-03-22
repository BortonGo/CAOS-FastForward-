#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dlist.h"

enum {
    MAGIC = 16,
};

static bool out_of_memory = false;

void *__real_calloc(size_t nmemb, size_t size);

void *__wrap_calloc(size_t nmemb, size_t size) {
    if (out_of_memory) {
        return NULL;
    }
    return __real_calloc(nmemb, size);
}

int main() {
    char buf[MAGIC];
    struct Node *cur = NULL;
    while (scanf("%10s", buf) == 1) {
        if (strcmp(buf, "next") == 0) {
            cur = cur->next;
            assert(cur);
        } else if (strcmp(buf, "prev") == 0) {
            cur = cur->prev;
            assert(cur);
        } else if (strcmp(buf, "pop") == 0) {
            assert(cur);
            struct Node *prev_or_next = cur->prev;
            if (prev_or_next == NULL) {
                prev_or_next = cur->next;
            }
            printf("%d\n", pop(cur));
            cur = prev_or_next;
        } else if (strncmp(buf, "push", 4) == 0) {
            int x;
            assert(scanf("%d", &x) == 1);
            if (buf[4] == '!') {
                out_of_memory = true;
                if (push(cur, x)) {
                    fprintf(stderr, "Expected NULL from push(...)");
                    return 1;
                }
                out_of_memory = false;
            } else {
                cur = push(cur, x);
                assert(cur);
            }
        } else {
            fprintf(stderr, "Unknown command %s\n", buf);
            return 1;
        }
    }
}
