#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define GC_DEBUG
void not_printf(const char *str, ...) {}
#ifdef GC_DEBUG
    #define gc_debug printf
#else
    #define gc_debug not_printf
#endif

typedef struct s_block {
    void *pointer;
    size_t size;
} t_block;

typedef struct s_gc {
    t_block *elements;
    size_t size;
} t_gc;

t_gc gc = {NULL, 0};


void *gc_malloc(size_t size) {
    gc_debug("GC_DEBUG: MALLOC\n");
    size_t p = gc.size;
    gc_debug("GC_DEBUG: CURRENT_SIZE: %u\n", gc.size);
    gc.size++;
    gc_debug("GC_DEBUG: TARGET_SIZE: %u\n", gc.size);
    gc.elements = (t_block*)realloc(gc.elements, gc.size * sizeof(t_block));
    gc_debug("GC_DEBUG: NEW BLOCK ADDRESS: %x\n", gc.elements);
    gc.elements[p].size = size;
    gc_debug("GC_DEBUG: WRITE BLOCK SIZE: %u\n", gc.elements[p].size);
    gc.elements[p].pointer = malloc(size);
    gc_debug("GC_DEBUG: WRITE NEW BLOCK ADDRESS: %x\n", gc.elements[p].pointer);
    return gc.elements[p].pointer;
}

void *gc_calloc(size_t size, size_t count) {
    gc_debug("GC_DEBUG: CALLOC\n");
    return gc_malloc(size * count);
}

void gc_free(void *pointer) {
    gc_debug("GC_DEBUG: FREE\n");
    for (size_t i = 0; i < gc.size; i++) {
        gc_debug("GC_DEBUG: SEARCHING POINTER: %u\n", i);
        if (pointer != gc.elements[i].pointer) continue;

        // Удаляем указатель
        gc_debug("GC_DEBUG: REMOVE POINTER: %x\n", gc.elements[i].pointer);
        free(gc.elements[i].pointer);
        
        // Смещаем все влево, заполняем пустое место
        // 123_56 -> 12356_
        gc_debug("GC_DEBUG: SHIFT POINTERS TO LEFT\n");
        for (size_t j = i; j < gc.size - 1; j++)
            gc.elements[i] = gc.elements[i + 1];

        // Вычитаем элемент
        gc_debug("GC_DEBUG: CURRENT SIZE: %u\n", gc.size);
        gc.size--;
        gc_debug("GC_DEBUG: TARGET SIZE: %u\n", gc.size);

        // Если что-то осталось, то меняем размер -1
        // 12356_ -> 12356
        if (gc.size) {
            gc.elements = realloc(gc.elements, gc.size);
            gc_debug("GC_DEBUG: SIZE NOT NULL, REALLOC ON: %x\n", gc.elements);
        // Иначе вообще очищаем
        // _ -> NULL
        } else {
            gc_debug("GC_DEBUG: SIZE IS NULL, JUST FREE\n");
            free(gc.elements);
        }
        break;
    }
}

void gc_freeall() {
    gc_debug("GC_DEBUG: FREEALL\n");
    for (int64_t i = gc.size - 1; i >= 0; i--) {
        gc_debug("GC_DEBUG: FREE %u ELEMENT\n", i);
        free(gc.elements[i].pointer);
    }
    gc_debug("GC_DEBUG: FREE ELEMENTS BUFFER\n");
    free(gc.elements);
    gc_debug("GC_DEBUG: UPDATE BUFFER SIZE TO 0\n");
    gc.size = 0;
}

int main() {
    int *a = (int*)gc_malloc(sizeof(int));
    int *b = (int*)gc_malloc(sizeof(int));

    *a = 6;
    *b = 7;
    printf("a+b=%d\n", *a + *b);

    gc_free(a);

    gc_freeall();

    return 0;
}
