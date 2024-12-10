#ifndef DIFICULTADES_H_
#define DIFICULTADES_H_

#include "simulador.h"

typedef enum {
	Facil,
	Normal,
	Dificil
} DificultadEstandar;

/* Devuelve un puntero a la dificultad pedida o NULL en caso de error. */
DatosDificultad* conseguir_dificultad_estandar(DificultadEstandar dificultad);

/* Devuelve un puntero a la copia de la dificultad pasada por parametro o NULL en caso de error. */
DatosDificultad* duplicar_dificultad(DatosDificultad* a_duplicar);

/* Libera la memoria reservada para la dificultad pasada por parametro.
   La dificultad debe de haber sido creada por conseguir_dificultad_estandar() o duplicar_dificultad().
   No debe ser nulo el parametro. */
void destruir_dificultad(DatosDificultad* dificultad);

#endif //DIFICULTADES_H_