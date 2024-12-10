#ifndef HEAP_H_
#define HEAP_H_

#include <stdlib.h>

typedef struct _heap heap_t;

/* Para heap maximal:
   Devuelve >0 si el primer elemento es mayor.
   Devuelve <0 si el primer elemento es menor.

   Para heap minimal:
   Devuelve <0 si el primer elemento es mayor.
   Devuelve >0 si el primer elemento es menor.

   Para ambos:
   Devuelve =0 si ambos elementos son iguales. */
typedef int (*heap_comparador)(void*, void*);

heap_t* heap_crear(heap_comparador comparador);

int heap_insertar(heap_t* heap, void* elemento);

size_t heap_cantidad_elementos(heap_t* heap);

void* heap_extraer_raiz(heap_t* heap);

void heap_destruir(heap_t* heap);

#endif //HEAP_H_