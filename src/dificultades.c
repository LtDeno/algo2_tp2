#include "dificultades.h"

#include <string.h>

#define MIN_INTENTOS 1
#define MAX_INTENTOS 100
#define MULTIPLICADOR_INTENTOS 3
#define MULTIPLICADOR_ACIERTO_PRIMER_INTENTO 2
#define PUNTAJE_BASE 3000
#define PUNTAJE_MINIMO 1
#define TAMANIO_STRINGS 10

#define STRING_INTENTO_LEJANO_SUPERIOR_FACIL "Estas errado por mas de 10 niveles por arriba del correcto."
#define STRING_INTENTO_CERCANO_SUPERIOR_FACIL "Estas errado por menos o igual 10 niveles por arriba del correcto."
#define STRING_INTENTO_LEJANO_INFERIOR_FACIL "Estas errado por mas de 10 niveles por abajo del correcto."
#define STRING_INTENTO_CERCANO_INFERIOR_FACIL "Estas errado por menos o igual 10 niveles por abajo del correcto."
#define STRING_INTENTO_ACERTADO_FACIL "Acertaste el nivel!"

#define STRING_INTENTO_SUPERIOR_NORMAL "Estas por arriba del nivel correcto."
#define STRING_INTENTO_INFERIOR_NORMAL "Estas por abajo del nivel correcto."
#define STRING_INTENTO_ACERTADO_NORMAL "Golazo!"

#define STRING_INTENTO_ERRONEO_DIFICIL "Mal."
#define STRING_INTENTO_ACERTADO_DIFICIL "Bien."

unsigned calculo_puntaje_estandar(unsigned cantidad_intentos){
	if (cantidad_intentos == MIN_INTENTOS){
		return PUNTAJE_BASE * MULTIPLICADOR_ACIERTO_PRIMER_INTENTO;
	} else if ((MIN_INTENTOS < cantidad_intentos) && (cantidad_intentos < MAX_INTENTOS)){
		return PUNTAJE_BASE - (cantidad_intentos * MULTIPLICADOR_INTENTOS);
	} else {
		return PUNTAJE_MINIMO;
	}
}

int verificador_nivel_estandar(unsigned intento, unsigned nivel_pokemon){
	return (int)intento - (int)nivel_pokemon;
}

const char* verificacion_a_string_izi(int resultado_verificacion){
	if (resultado_verificacion > 10){
		return STRING_INTENTO_LEJANO_SUPERIOR_FACIL;
	} else if ((0 < resultado_verificacion) && (resultado_verificacion <= 10)){
		return STRING_INTENTO_CERCANO_SUPERIOR_FACIL;
	} else if (-10 > resultado_verificacion){
		return STRING_INTENTO_LEJANO_INFERIOR_FACIL;
	} else if ((-10 <= resultado_verificacion) && (resultado_verificacion < 0)){
		return STRING_INTENTO_CERCANO_INFERIOR_FACIL;
	} else {
		return STRING_INTENTO_ACERTADO_FACIL;
	}
}

const char* verificacion_a_string_gg(int resultado_verificacion){
	if (resultado_verificacion > 0){
		return STRING_INTENTO_SUPERIOR_NORMAL;
	} else if (resultado_verificacion < 0){
		return STRING_INTENTO_INFERIOR_NORMAL;
	} else {
		return STRING_INTENTO_ACERTADO_NORMAL;
	}
}

/* Como se nota que juego lol... */
const char* verificacion_a_string_super_izi(int resultado_verificacion){
	if (resultado_verificacion == 0){
		return STRING_INTENTO_ACERTADO_DIFICIL;
	} else {
		return STRING_INTENTO_ERRONEO_DIFICIL;
	}
}

DatosDificultad* dificultad_facil(){
	DatosDificultad* facil = calloc(1, sizeof(DatosDificultad));
	if (!facil){
		return NULL;
	}

	char* nombre = calloc(TAMANIO_STRINGS, sizeof(char));
	if (!nombre){
		free(facil);
		return NULL;
	}
	strcpy(nombre, "Facil");

	facil->nombre = nombre;
	facil->calcular_puntaje = calculo_puntaje_estandar;
	facil->verificar_nivel = verificador_nivel_estandar;
	facil->verificacion_a_string = verificacion_a_string_izi;

	return facil;
}

DatosDificultad* dificultad_normal(){
	DatosDificultad* normal = calloc(1, sizeof(DatosDificultad));
	if (!normal){
		return NULL;
	}

	char* nombre = calloc(TAMANIO_STRINGS, sizeof(char));
	if (!nombre){
		free(normal);
		return NULL;
	}
	strcpy(nombre, "Normal");

	normal->nombre = nombre;
	normal->calcular_puntaje = calculo_puntaje_estandar;
	normal->verificar_nivel = verificador_nivel_estandar;
	normal->verificacion_a_string = verificacion_a_string_gg;

	return normal;
}

DatosDificultad* dificultad_dificil(){
	DatosDificultad* dificil = calloc(1, sizeof(DatosDificultad));
	if (!dificil){
		return NULL;
	}

	char* nombre = calloc(TAMANIO_STRINGS, sizeof(char));
	if (!nombre){
		free(dificil);
		return NULL;
	}
	strcpy(nombre, "Dificil");

	dificil->nombre = nombre;
	dificil->calcular_puntaje = calculo_puntaje_estandar;
	dificil->verificar_nivel = verificador_nivel_estandar;
	dificil->verificacion_a_string = verificacion_a_string_super_izi;

	return dificil;
}

DatosDificultad* dificultad_vacia(){
	return calloc(1, sizeof(DatosDificultad));
}

DatosDificultad* conseguir_dificultad_estandar(DificultadEstandar dificultad){
	switch (dificultad){
		case Facil:
			return dificultad_facil();
		case Normal:
			return dificultad_normal();
		case Dificil:
			return dificultad_dificil();
		default:
			return dificultad_vacia();
	}

	return NULL;
}

DatosDificultad* duplicar_dificultad(DatosDificultad* a_duplicar){
	if (!a_duplicar){
		return NULL;
	}

	DatosDificultad* nueva = calloc(1, sizeof(DatosDificultad));
	if (!nueva){
		return NULL;
	}

	char* nombre = calloc(strlen(a_duplicar->nombre) + 1, sizeof(char));
	if (!nombre){
		free(nueva);
		return NULL;
	}
	strcpy(nombre, a_duplicar->nombre);
	nueva->nombre = nombre;

	nueva->calcular_puntaje = a_duplicar->calcular_puntaje;
	nueva->verificar_nivel = a_duplicar->verificar_nivel;
	nueva->verificacion_a_string = a_duplicar->verificacion_a_string;

	return nueva;
}

void destruir_dificultad(DatosDificultad* dificultad){
	free((void*)dificultad->nombre);
	free(dificultad);
}
