#ifndef __GC_ALLOC_H__
#define __GC_ALLOC_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void not_printf(const char*, ...);
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

extern t_gc gc;

void *gc_malloc(size_t);
void *gc_calloc(size_t, size_t);
void gc_free(void*);
void gc_freeall(void);

#endif