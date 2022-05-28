#include "gc_alloc.h"

t_gc gc;

void not_printf(const char *str, ...) { }

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

    if (gc.elements == NULL) {
        gc_debug("GC_DEBUG: GC ELEMENTS IS EMPTY\n");
        return;
    }

    if (pointer == NULL) {
        gc_debug("GC_DEBUG: INPUT POINTER IS NULL\n");
        return;
    }

    for (size_t i = 0; i < gc.size; i++) {
        gc_debug("GC_DEBUG: SEARCHING POINTER ON: %u\n", i);
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

    if (gc.elements == NULL) {
        gc_debug("GC_DEBUG: GC ELEMENTS IS EMPTY\n");
        return;
    }

    for (int64_t i = gc.size - 1; i >= 0; i--) {
        gc_debug("GC_DEBUG: FREE %u ELEMENT\n", i);

        if (gc.elements[i].pointer == NULL) {
            gc_debug("GC_DEBUG: POINTER IS NULL\n");
            continue;
        }

        free(gc.elements[i].pointer);
    }
    gc_debug("GC_DEBUG: FREE ELEMENTS BUFFER\n");
    free(gc.elements);
    gc_debug("GC_DEBUG: UPDATE BUFFER SIZE TO 0\n");
    gc.size = 0;
}
