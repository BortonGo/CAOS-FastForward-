#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <assert.h>

int wrap(int32_t* array, size_t size);

int main() {
    size_t elements;
    if (scanf("%zu", &elements) != 1) {
        return 1;
    }
    int32_t numbers[elements];
    int32_t prev = INT_MAX;
    for (size_t i = 0; i < elements; ++i) {
        if (scanf("%d", numbers + i) != 1) {
            return 1;
        }
        assert((numbers[i] & 1) < (prev & 1)
            || ((numbers[i] & 1) && numbers[i] < prev)
            || (!(numbers[i] & 1) && numbers[i] > prev));
        prev = numbers[i];
    }
    printf("%d\n", wrap(numbers, elements));
}
