#include "heap.h"

#include <stdlib.h>
#include <stdio.h>

#define TAMANIO_INI_VECTOR 20


struct _heap{
	void** vector_elementos;
	int tope_vector;
	size_t tamanio_vector;
	heap_comparador comparador;
};

void swap(void** vector, int pos1, int pos2){
	void* aux = vector[pos2];
	vector[pos2] = vector[pos1];
	vector[pos1] = aux; 
}

int pos_padre(int posicion){
	int pos_pa = posicion-2;

	if (pos_pa%2 == 0){
		pos_pa = pos_pa/2;
	} else{
		pos_pa = (pos_pa+1)/2;
	}

	return pos_pa;
}

void sift_up(heap_t* heap, int posicion){
	if (!heap || !heap->vector_elementos || (posicion <= 0)){
		return;
	}

	int pos_pa = posicion-1;

	if (heap->comparador(heap->vector_elementos[pos_pa], heap->vector_elementos[posicion]) < 0){
		swap(heap->vector_elementos, posicion, pos_pa);
		sift_up(heap, pos_pa);
	}
	return;	
}

void sift_down(heap_t* heap, int posicion){
	if (!heap || !heap->vector_elementos || (heap->tope_vector-1 <= posicion)){
		return;
	}

	int pos_a_intercambiar = posicion+1;

	if (heap->comparador(heap->vector_elementos[posicion], heap->vector_elementos[pos_a_intercambiar]) < 0){
		swap(heap->vector_elementos, posicion, pos_a_intercambiar);
		sift_down(heap, pos_a_intercambiar);
	}
	return;
}

int resize(heap_t* heap){
	if (!heap){
		return -1;
	}

	void** vector_aux = realloc(heap->vector_elementos, (heap->tamanio_vector*2) * sizeof(void*));
	if (!vector_aux){
		return -1;
	}

	heap->vector_elementos = vector_aux;
	heap->tamanio_vector *= 2;

	return 0;
}

heap_t* heap_crear(heap_comparador comparador){
	if (!comparador){
		return NULL;
	}

	heap_t* heap = calloc(1, sizeof(heap_t));
	if (!heap){
		return NULL;
	}

	heap->vector_elementos = calloc(TAMANIO_INI_VECTOR, sizeof(void*));
	if (!heap->vector_elementos){
		free(heap);
		return NULL;
	}
	
	heap->tope_vector = 0;
	heap->tamanio_vector = TAMANIO_INI_VECTOR;
	heap->comparador = comparador;

	return heap;
}

size_t heap_cantidad_elementos(heap_t* heap){
	if (!heap){
		return 0;
	}
	return (size_t)heap->tope_vector;
}

int heap_insertar(heap_t* heap, void* elemento){
	if (!heap || !heap->vector_elementos){
		return -1;
	}

	if (heap->tope_vector == (int)(heap->tamanio_vector-1)){
		int resultado = resize(heap);
		if (resultado != 0){
			return -1;
		}
	}

	heap->vector_elementos[heap->tope_vector] = elemento;
	sift_up(heap, heap->tope_vector);
	heap->tope_vector++;

	return 0;
}

void* heap_extraer_raiz(heap_t* heap){
	if (!heap || !heap->vector_elementos || (heap->tope_vector == 0)){
		return NULL;
	}

	void* elem_a_devolver = heap->vector_elementos[0];
	heap->tope_vector--;
	swap(heap->vector_elementos, 0, heap->tope_vector);
	sift_down(heap, 0);
	heap->vector_elementos[heap->tope_vector] = NULL;
	
	return elem_a_devolver;
}

void heap_destruir(heap_t* heap){
	if (!heap){
		return;
	}

	free(heap->vector_elementos);
	free(heap);
}
