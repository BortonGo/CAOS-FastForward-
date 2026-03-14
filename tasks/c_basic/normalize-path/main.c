#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../common-files/include/simpleasan.h"

void normalize_path(char *path);

int main() {
    char *buf = NULL;
    size_t bufsize = 0;
    int len;
    while ((len = getline(&buf, &bufsize, stdin)) != -1) {
        while (len > 0 && isspace(buf[len - 1])) {
            --len;
        }
        buf[len] = 0;
        char *tmpbuf = malloc_protected(len + 1);
        memcpy(tmpbuf, buf, len + 1);
        normalize_path(tmpbuf);
        printf(">%s<\n", tmpbuf);
        free_protected(tmpbuf);
    }
    free(buf);
    return 0;
}
