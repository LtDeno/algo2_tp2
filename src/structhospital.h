#ifndef STRUCTHOSPITAL_H_
#define STRUCTHOSPITAL_H_

#include "lista.h"
#include "abb.h"
#include "hospital.h"

#include <stdlib.h>

struct _hospital_pkm_t{
    lista_t* lista_entrenadores;
    size_t cantidad_pokemones;
};

struct _pkm_t{
    char* nombre;
    size_t nivel;
    entrenador_t* entrenador;
};

struct _entrenador_t{
    char* nombre;
    int id;
    abb_t* pokemones;
};


#endif //STRUCTHOSPITAL_H