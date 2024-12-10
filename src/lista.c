#include "lista.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

lista_t* lista_crear(){
    return calloc(1, sizeof(lista_t));
}

lista_t* lista_insertar(lista_t* lista, void* elemento){
    if (!lista){
        return NULL;
    }
    nodo_t* nuevo_nodo = (nodo_t*)calloc(1, sizeof(nodo_t));
    if (!nuevo_nodo){
        return NULL;
    }
    nuevo_nodo->elemento = elemento;

    if (lista->cantidad <= 0){
        lista->nodo_inicio = nuevo_nodo;
    } else {
        lista->nodo_fin->siguiente = nuevo_nodo;
    }

    lista->nodo_fin = nuevo_nodo;
    lista->cantidad++;

    return lista;
}

lista_t* lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
    if (!lista){
        return NULL;
    }

    if (posicion >= lista->cantidad){
        return lista_insertar(lista, elemento);
    }

    nodo_t* nuevo_nodo = calloc(1, sizeof(nodo_t));
    if (!nuevo_nodo){
        return NULL;
    }
    nuevo_nodo->elemento = elemento;

    if (posicion == 0){
        nuevo_nodo->siguiente = lista->nodo_inicio;
        lista->nodo_inicio = nuevo_nodo;
        lista->cantidad++;
        
        return lista;
    }

    nodo_t* nodo_aux = lista->nodo_inicio;
    for (size_t i = 1 ; i < posicion ; i++){
        nodo_aux = nodo_aux->siguiente;
    }

    nuevo_nodo->siguiente = nodo_aux->siguiente;
    nodo_aux->siguiente = nuevo_nodo;
    lista->cantidad++;

    return lista;
}

void* lista_quitar(lista_t* lista){
    if (!lista){
        return NULL;
    }

    void* elemento_a_devolver = NULL;

    if (lista_vacia(lista)){
        return elemento_a_devolver;
    }

    nodo_t* anteultimo_nodo = NULL;
    if (lista->cantidad > 1){
        anteultimo_nodo = lista->nodo_inicio;
        for (size_t i = 1 ; i < (lista->cantidad - 1) ; i++){
            anteultimo_nodo = anteultimo_nodo->siguiente;
        }

        elemento_a_devolver = lista->nodo_fin->elemento;
        anteultimo_nodo->siguiente = NULL;

    } else {
        lista->nodo_inicio = NULL;
        elemento_a_devolver = lista->nodo_fin->elemento;
    }

    free(lista->nodo_fin);
    lista->nodo_fin = anteultimo_nodo;
    lista->cantidad--;

    return elemento_a_devolver;
}

void* lista_quitar_de_posicion(lista_t* lista, size_t posicion){
    if (!lista || lista_vacia(lista)){
        return NULL;
    }
    if (posicion >= (lista->cantidad - 1)){
        return lista_quitar(lista);
    }

    void* elemento_a_devolver = NULL;
    nodo_t* nodo_a_quitar = NULL;
    nodo_t* nodo_previo_al_quitado = lista->nodo_inicio;

    if (posicion == 0){
        nodo_a_quitar = lista->nodo_inicio;
        lista->nodo_inicio = nodo_a_quitar->siguiente;

    } else {
        for (size_t i = 1 ; i < posicion ; i ++){
            nodo_previo_al_quitado = nodo_previo_al_quitado->siguiente;
        }

        nodo_a_quitar = nodo_previo_al_quitado->siguiente;
        nodo_previo_al_quitado->siguiente = nodo_a_quitar->siguiente;
    }

    elemento_a_devolver = nodo_a_quitar->elemento;
    free(nodo_a_quitar);
    lista->cantidad--;

    return elemento_a_devolver;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
    if (!lista || (posicion >= lista->cantidad)){
        return NULL;
    }

    nodo_t* nodo_con_elemento_a_dar = lista->nodo_inicio;
    for (size_t i = 0 ; i < posicion ; i++){
        nodo_con_elemento_a_dar = nodo_con_elemento_a_dar->siguiente;
    }

    return nodo_con_elemento_a_dar->elemento;
}

void* lista_primero(lista_t* lista){
    if (!lista || lista_vacia(lista)){
        return NULL;
    }

    return lista->nodo_inicio->elemento;
}

void* lista_ultimo(lista_t* lista){
    if (!lista || lista_vacia(lista)){
        return NULL;
    }

    return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t* lista){
    if (!lista || (lista->cantidad <= 0)){
        return true;
    }

    return false;
}

size_t lista_tamanio(lista_t* lista){
    if (!lista){
        return 0;
    }

    return lista->cantidad;
}

void lista_destruir(lista_t* lista){
    if (!lista){
        return;
    }

    nodo_t* nodo_a_borrar = lista->nodo_inicio;

    while(nodo_a_borrar){
        lista->nodo_inicio = lista->nodo_inicio->siguiente;
        free(nodo_a_borrar);
        nodo_a_borrar = lista->nodo_inicio;
    }
    
    free(lista);
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){
    if (!lista){
        return NULL;
    }

    lista_iterador_t* listerador = (lista_iterador_t*)calloc(1, sizeof(lista_iterador_t ));
    if (!listerador){
        return NULL;
    }

    listerador->lista = lista;
    if (lista_vacia(listerador->lista)){
        return listerador;
    } else {
        listerador->corriente = listerador->lista->nodo_inicio;
    }

    return listerador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
    if (!iterador || lista_vacia(iterador->lista) || !(iterador->corriente)){
        return false;
    }

    return true;
}

bool lista_iterador_avanzar(lista_iterador_t* iterador){
    if (!iterador || lista_vacia(iterador->lista) || !(iterador->corriente)){
        return false;
    }

    iterador->corriente = iterador->corriente->siguiente;
    if (!(iterador->corriente)){
        return false;
    }

    return true;
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador){
    if (!iterador || !(iterador->corriente)){
        return NULL;
    }

    return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t* iterador){
    if (!iterador){
        return;
    }

    free(iterador);
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto){
    if (!lista || !funcion || lista_vacia(lista)){
        return 0;
    }

    lista_iterador_t* listerador = lista_iterador_crear(lista);
    if (!listerador){
        return 0;
    }

    size_t contador = 0;
    bool seguir_aplicando_funcion = true;
    while (lista_iterador_tiene_siguiente(listerador) && seguir_aplicando_funcion){
        seguir_aplicando_funcion = funcion( lista_iterador_elemento_actual(listerador) , contexto);
        contador++;
        lista_iterador_avanzar(listerador);
    }

    free(listerador);
    return contador;
}
