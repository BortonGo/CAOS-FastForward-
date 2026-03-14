#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int mystrcmp(const char *p1, const char *p2);

int main(void)
{
    char buf1[1024];
    char buf2[1024];
    while (fgets(buf1, sizeof(buf1), stdin)&&fgets(buf2, sizeof(buf2), stdin)){
        size_t len1 = strlen(buf1);
        size_t len2 = strlen(buf2);
        if (len1 > 0 && buf1[len1 - 1] == '\n') {
            buf1[--len1] = 0;
        }
        if (len2 > 0 && buf2[len2 - 1] == '\n') {
            buf2[--len2] = 0;
        }
        // for valgrind
        char *s1 = strdup(buf1);
        char *s2 = strdup(buf2);
        int r = mystrcmp(s1, s2);
        if (r < 0) r = -1;
        if (r > 0) r = 1;
        printf("%d\n", r);
        free(s1);
        free(s2);
    }
}
