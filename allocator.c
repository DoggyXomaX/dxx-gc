#include "lib/gc_alloc.h"

int main() {
    int *a = (int*)gc_malloc(sizeof(int));
    int *b = (int*)gc_malloc(sizeof(int));
    int *c = (int*)gc_malloc(sizeof(int));

    *a = 6;
    *b = 7;
    *c = 99;
    printf("a + b + c = %d\n", *a + *b + *c);

    gc_free(c);

    gc_freeall();

    return 0;
}
