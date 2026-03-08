#include <stdlib.h>
#include <stdio.h>

static int compar(const void *a, const void *b) {
    int x = *((const int *)a);
    int y = *((const int *)b);
    if ((x & 1) != (y & 1)) {
    	return (y & 1) - (x & 1);
    }
	return x & 1 ? y - x : x - y;
}

int bsearch40(int *arr, size_t n) {
	int key = 40;
	void* res = bsearch(&key, arr, n, sizeof(arr[0]), compar);
	return res == NULL ? -1 : (int*)res - arr;
}
