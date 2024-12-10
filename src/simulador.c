#include "simulador.h"
#include "hospital.h"
#include "structhospital.h"
#include "heap.h"
#include "dificultades.h"

#include <stdio.h>
#include <string.h>

#define DIFICULTAD_INICIAL 1
#define ID_SI_DIFICULTAD_NO_EXISTE -1
#define MIN_ID_DIFICULTAD 0
#define POSICION_ENTRENADOR_COMO_SI_FUESE_COLA 0

struct _simulador_t{
	hospital_t* hospital;

	lista_t* sala_de_espera_entrenadores;
	heap_t* sala_de_espera_pokemon;
	pokemon_t* pokemon_a_atender;

	EstadisticasSimulacion* stats;

	lista_t* dificultades;
	DatosDificultad* dificultad_actual;

	bool simulacion_activa;
	unsigned cant_intentos_actual;
};

typedef struct busqueda_nombre{
	const char* nombre_buscado;
	bool encontrado;
}busqueda_nombre_t;


/*  Funcion de iterador de lista.
    Inserta el elemento pasado por parametro
    en la lista, pasada por segundo parametro.

    Devuelve true siempre que inserte correctamente en la lista. */
bool insertar_elemento_en_lista(void* elemento, void* _lista){
	if (!elemento || !_lista){
		return false;
	}

	lista_t* lista = _lista;
	if (lista_insertar(lista, elemento)){
		return true;
	}

	return false;
}

/*  Funcion de iterador de lista.
    Compara los nombres de ambos parametros, siendo este primero
    un DatosDificultad* y el segundo una busqueda_nombre_t*
 
    Devuelve false si coinciden en nombre. */
bool es_nombre_dificultad_no_coincidente(void* _dificultad, void* _busqueda_nombre){
	if (!_dificultad || !_busqueda_nombre){
		return false;
	}
	
	DatosDificultad* dificultad = _dificultad;
	busqueda_nombre_t* auxiliar = _busqueda_nombre;

	if (strcmp(dificultad->nombre, auxiliar->nombre_buscado) == 0){
		auxiliar->encontrado = true;
		return false;
	}

	return true;
}

/*  Copia la lista pasada por parametro en una nueva lista
    
    Devuelve NULL si hubo error. Recordar liberar esta lista. */
lista_t* copiar_lista(lista_t* lista){
	if (!lista){
		return NULL;
	}

	lista_t* copia = calloc(1, sizeof(lista_t));
	if (!copia){
		return NULL;
	}

	lista_con_cada_elemento(lista, insertar_elemento_en_lista, (void*)copia);
	return copia;
}

/*  Crea una estructura EstadisticasSimulacion y la inicializa con los datos del hospital. 
    Devuelve su direccion de memoria.
    
    Devuelve NULL si hubo error. */
EstadisticasSimulacion* cargar_estadisticas_iniciales(hospital_t* hospital){
	EstadisticasSimulacion* stats = calloc(1, sizeof(EstadisticasSimulacion));
	if (!stats){
		return NULL;
	}

	stats->entrenadores_totales = (unsigned int)lista_tamanio(hospital->lista_entrenadores);
	stats->pokemon_totales = (unsigned int)hospital->cantidad_pokemones;
	return stats;
}

/*  Comparador para ABB. */
int comparador_pokemon_nivel(void* _poke1, void* _poke2){
	pokemon_t* poke1 = _poke1;
	pokemon_t* poke2 = _poke2;
	return (int)poke2->nivel - (int)poke1->nivel;
}

/*  Crea una lista nueva con las dificultades estandar como elemento. */
lista_t* conseguir_lista_dificultades_estandar(){
	lista_t* dificultades = lista_crear();
	if (!dificultades){
		return NULL;
	}

	lista_insertar(dificultades, conseguir_dificultad_estandar(Facil));
	lista_insertar(dificultades, conseguir_dificultad_estandar(Normal));
	lista_insertar(dificultades, conseguir_dificultad_estandar(Dificil));

	return dificultades;	
}

/*  Funcion de iterador de lista.
    Destruye la dificultad pasada por primer parametro.

    Devuelve true, o false si hubo error. */
bool destruir_dificultades(void* _dificultad, void* _nada){
	if (!_dificultad){
		return false;
	}

	DatosDificultad* dificultad = _dificultad;
	destruir_dificultad(dificultad);
	
	return true;
}

/*  Funcion de iterador de ABB (tambien puede ser de lista pero no importa)
    Inserta el elemento pasado por primer parametro en el heap del
    segundo parametro.

    Devuelve true, o false si hay error. */    
bool de_abb_a_heap(void* _elemento, void* _heap){
	if (!_elemento || !_heap){
		return false;
	}
	heap_t* heap = _heap;
	heap_insertar(heap, _elemento);

	return true;
}

/*  Itera la lista de dificultades (segundo parametro) hasta
    encontrar una dificultad que coicida en nombre con la del
    primer parametro.

    Devuelve true si existe la dificultad. False en caso contrario */
bool existe_dificultad(DatosDificultad* dificultad, lista_t* dificultades){
	if (!dificultad || !dificultades){
		return false;
	}

	busqueda_nombre_t* auxiliar = calloc(1, sizeof(busqueda_nombre_t));
	if (!auxiliar){
		return false;
	}
	auxiliar->nombre_buscado = dificultad->nombre;
	auxiliar->encontrado = false;

	lista_con_cada_elemento(dificultades, es_nombre_dificultad_no_coincidente, (void*)auxiliar);

	if (auxiliar->encontrado){
		free(auxiliar);
		return true;
	} else {
		free(auxiliar);
		return false;
	}

	return true;
}



/* Simulaciones */
ResultadoSimulacion simulacion_finalizar(simulador_t* simulador){
	if (!simulador){
		return ErrorSimulacion;
	}
	simulador->stats->cantidad_eventos_simulados++;

	simulador->simulacion_activa = false;
	return ExitoSimulacion;
}

ResultadoSimulacion simulacion_obtener_estadisticas(simulador_t* simulador, void* _datos_estadisticas){
	if (!simulador || !_datos_estadisticas){
		return ErrorSimulacion;
	}

	simulador->stats->cantidad_eventos_simulados++;
	
	EstadisticasSimulacion* estadisticas = _datos_estadisticas;

	estadisticas->entrenadores_atendidos = simulador->stats->entrenadores_atendidos;
	estadisticas->entrenadores_totales = simulador->stats->entrenadores_totales;

	estadisticas->pokemon_atendidos = simulador->stats->pokemon_atendidos;
	estadisticas->pokemon_en_espera = simulador->stats->pokemon_en_espera;
	estadisticas->pokemon_totales = simulador->stats->pokemon_totales;

	estadisticas->puntos = simulador->stats->puntos;
	estadisticas->cantidad_eventos_simulados = simulador->stats->cantidad_eventos_simulados;
	
	return ExitoSimulacion;
}

ResultadoSimulacion simulacion_atender_proximo_entrenador(simulador_t* simulador){
	if (!simulador){
		return ErrorSimulacion;
	}

	simulador->stats->cantidad_eventos_simulados++;

	entrenador_t* entrenador_a_atender = lista_primero(simulador->sala_de_espera_entrenadores);
	if (!entrenador_a_atender){
		return ErrorSimulacion;
	}

	abb_con_cada_elemento(entrenador_a_atender->pokemones, PREORDEN, de_abb_a_heap, (void*)simulador->sala_de_espera_pokemon);

	lista_quitar_de_posicion(simulador->sala_de_espera_entrenadores, POSICION_ENTRENADOR_COMO_SI_FUESE_COLA);
	simulador->stats->entrenadores_atendidos++;

	if (!simulador->pokemon_a_atender){
		simulador->pokemon_a_atender = (pokemon_t*)heap_extraer_raiz(simulador->sala_de_espera_pokemon);
	}

	simulador->stats->pokemon_en_espera = (unsigned int)heap_cantidad_elementos(simulador->sala_de_espera_pokemon);

	return ExitoSimulacion;
}

ResultadoSimulacion simulacion_info_pokemon(simulador_t* simulador, void* _datos_pokemon){
	if (!simulador || !_datos_pokemon){
		return ErrorSimulacion;
	}

	simulador->stats->cantidad_eventos_simulados++;
	
	InformacionPokemon* info_pokemon = _datos_pokemon;

	if (!simulador->pokemon_a_atender){
		info_pokemon->nombre_pokemon = NULL;
		info_pokemon->nombre_entrenador = NULL;
		return ErrorSimulacion;
	}

	info_pokemon->nombre_pokemon = simulador->pokemon_a_atender->nombre;
	info_pokemon->nombre_entrenador = simulador->pokemon_a_atender->entrenador->nombre;

	return ExitoSimulacion;
}

ResultadoSimulacion simulacion_adivinar_nivel(simulador_t* simulador, void* _datos_intento){
	if (!simulador || !_datos_intento){
		return ErrorSimulacion;
	}

	simulador->stats->cantidad_eventos_simulados++;

	if (!simulador->pokemon_a_atender){
		return ErrorSimulacion;
	}

	simulador->cant_intentos_actual++;
	Intento* adivinanza = _datos_intento;
	int resultado_adivinanza = simulador->dificultad_actual->verificar_nivel(adivinanza->nivel_adivinado, (unsigned int)simulador->pokemon_a_atender->nivel);
	
	if (resultado_adivinanza == 0){
		adivinanza->es_correcto = true;

		simulador->stats->puntos += simulador->dificultad_actual->calcular_puntaje(simulador->cant_intentos_actual);
		simulador->stats->pokemon_atendidos++;

		simulador->pokemon_a_atender = (pokemon_t*)heap_extraer_raiz(simulador->sala_de_espera_pokemon);
		simulador->stats->pokemon_en_espera = (unsigned int)heap_cantidad_elementos(simulador->sala_de_espera_pokemon);

		simulador->cant_intentos_actual = 0;		
	} else {
		adivinanza->es_correcto = false;	
	}
	adivinanza->resultado_string = simulador->dificultad_actual->verificacion_a_string(resultado_adivinanza);

	return ExitoSimulacion;
}

ResultadoSimulacion simulacion_agregar_dificultad(simulador_t* simulador, void* _datos_dificultad){
	if (!simulador || !_datos_dificultad){
		return ErrorSimulacion;
	}

	simulador->stats->cantidad_eventos_simulados++;

	DatosDificultad* dificultad = _datos_dificultad;
	if (!dificultad->nombre){
		return ErrorSimulacion;
	}

	if (existe_dificultad(dificultad, simulador->dificultades)){
		return ErrorSimulacion;
	}
	
	DatosDificultad* copia_dificultad = duplicar_dificultad(dificultad);
	if (!copia_dificultad){
		return ErrorSimulacion;
	}

	if(!lista_insertar(simulador->dificultades, (void*)copia_dificultad)){
		destruir_dificultad(copia_dificultad);
		return ErrorSimulacion;
	}

	return ExitoSimulacion;
}

ResultadoSimulacion simulacion_seleccionar_dificultad(simulador_t* simulador, void* _datos_id){
	if (!simulador || !_datos_id){
		return ErrorSimulacion;
	}

	simulador->stats->cantidad_eventos_simulados++;
	
	int* id = _datos_id;
	if ((*id < MIN_ID_DIFICULTAD) || (*id >= lista_tamanio(simulador->dificultades))){
		return ErrorSimulacion;
	}

	simulador->dificultad_actual = (DatosDificultad*)lista_elemento_en_posicion(simulador->dificultades, (size_t)*id);

	return ExitoSimulacion;
}

ResultadoSimulacion simulacion_obtener_info_dificultad(simulador_t* simulador, void* _datos_info_dificultad){
	if (!simulador || !_datos_info_dificultad){

		return ErrorSimulacion;
	}

	simulador->stats->cantidad_eventos_simulados++;

	InformacionDificultad* info = _datos_info_dificultad;
	info->en_uso = false;
	if ((info->id < 0) || (info->id >= lista_tamanio(simulador->dificultades))){
		info->nombre_dificultad = NULL;
		info->id = ID_SI_DIFICULTAD_NO_EXISTE;
		return ErrorSimulacion;
	}

	DatosDificultad* dificultad = lista_elemento_en_posicion(simulador->dificultades, (size_t)info->id);
	if (!dificultad){
		return ErrorSimulacion;
	}

	info->nombre_dificultad = dificultad->nombre;

	if (strcmp(simulador->dificultad_actual->nombre, dificultad->nombre) == 0){
		info->en_uso = true;
	}

	return ExitoSimulacion;
}



simulador_t* simulador_crear(hospital_t* hospital){
	if (!hospital){
		return NULL;
	}

	simulador_t* simulador = calloc(1, sizeof(simulador_t));
	if (!simulador){
		return NULL;
	}

	simulador->hospital = hospital;
	simulador->simulacion_activa = true;

	simulador->sala_de_espera_entrenadores = copiar_lista(hospital->lista_entrenadores);
	if (!simulador->sala_de_espera_entrenadores){
		free(simulador);
		return NULL;
	}

	simulador->sala_de_espera_pokemon = heap_crear(comparador_pokemon_nivel);
	if (!simulador->sala_de_espera_pokemon){
		lista_destruir(simulador->sala_de_espera_entrenadores);
		free(simulador);
		return NULL;
	}	

	simulador->stats = cargar_estadisticas_iniciales(hospital);
	if (!simulador->stats){
		lista_destruir(simulador->sala_de_espera_entrenadores);
		heap_destruir(simulador->sala_de_espera_pokemon);
		free(simulador);
		return NULL;
	}

	simulador->dificultades = conseguir_lista_dificultades_estandar();
	if (!simulador->dificultades){
		lista_destruir(simulador->sala_de_espera_entrenadores);
		heap_destruir(simulador->sala_de_espera_pokemon);
		free(simulador->stats);
		free(simulador);
		return NULL;
	}

	simulador->dificultad_actual = lista_elemento_en_posicion(simulador->dificultades, DIFICULTAD_INICIAL);

	return simulador;
}

ResultadoSimulacion simulador_simular_evento(simulador_t* simulador, EventoSimulacion evento, void* datos){
	if (!simulador || !simulador->simulacion_activa){
		return ErrorSimulacion;
	}

	switch (evento){
		case ObtenerEstadisticas:
			return simulacion_obtener_estadisticas(simulador, datos);
		case AtenderProximoEntrenador:
			return simulacion_atender_proximo_entrenador(simulador);
		case ObtenerInformacionPokemonEnTratamiento:
			return simulacion_info_pokemon(simulador, datos);
		case AdivinarNivelPokemon:
			return simulacion_adivinar_nivel(simulador, datos);
		case AgregarDificultad:
			return simulacion_agregar_dificultad(simulador, datos);
		case SeleccionarDificultad:
			return simulacion_seleccionar_dificultad(simulador, datos);
		case ObtenerInformacionDificultad:
			return simulacion_obtener_info_dificultad(simulador, datos);
		case FinalizarSimulacion:
			return simulacion_finalizar(simulador);	
		default:
			return ErrorSimulacion;
	}
}

void simulador_destruir(simulador_t* simulador){
	if (!simulador){
		return;
	}

	lista_destruir(simulador->sala_de_espera_entrenadores);
	heap_destruir(simulador->sala_de_espera_pokemon);
	free(simulador->stats);

	hospital_destruir(simulador->hospital);

	lista_con_cada_elemento(simulador->dificultades, destruir_dificultades, NULL);
	lista_destruir(simulador->dificultades);

	free(simulador);
}

void heap_imprimir_nivel_pokemon(heap_t* heap){
	size_t tamanio = heap_cantidad_elementos(heap);
	for (size_t i = 0; i < tamanio; i++){
		pokemon_t* poke_actual = heap->vector_elementos[i];
		printf("\n%lu", poke_actual->nivel);
	}
}