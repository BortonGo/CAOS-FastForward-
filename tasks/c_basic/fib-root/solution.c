#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_FIB 100

int main(void) {
    uint64_t fib[MAX_FIB];
    int n = 0;

    fib[n++] = 0;
    fib[n++] = 1;
    fib[n++] = 1;

    uint64_t next;
    while (!__builtin_add_overflow(fib[n - 1], fib[n - 2], &next)) {
        fib[n++] = next;
    }

    uint64_t x;
    while (scanf("%" SCNx64, &x) == 1) {
        int left = 0;
        int right = n - 1;
        int ans = 0;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (fib[mid] <= x) {
                ans = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        printf("%x\n", ans - 1);
    }

    return 0;
}