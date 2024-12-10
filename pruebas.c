#include "pa2mm.h"
#include "src/hospital.h"
#include "src/simulador.h"

#include "string.h"
#include <stdbool.h>

bool ignorar_pokemon(pokemon_t* p){
    p = p;
    return true;
}

/* No intenten esto en sus casas */
/* Ya vamos a ver como evitar esto en el TDA Lista */
struct{
    pokemon_t* pokemon[500];
    size_t cantidad;
} acumulados;

void resetear_acumulados(){
    acumulados.cantidad = 0;
}

bool acumular_pokemon(pokemon_t* p){
    acumulados.pokemon[acumulados.cantidad] = p;
    acumulados.cantidad++;
    return true;
}

bool acumular_pokemon_hasta_miltank(pokemon_t* p){
    acumulados.pokemon[acumulados.cantidad] = p;
    acumulados.cantidad++;
    return strcmp(pokemon_nombre(p), "miltank");
}

bool acumulados_en_orden_correcto(){
    if(acumulados.cantidad < 2)
        return true;
    pokemon_t* anterior = acumulados.pokemon[0];
    for(int i=1;i<acumulados.cantidad;i++){
        pokemon_t* actual =  acumulados.pokemon[i];
        if(strcmp(pokemon_nombre(anterior), pokemon_nombre(actual)) > 0)
            return false;
    }
    return true;
}

unsigned calcular_puntaje(unsigned cantidad_intentos){
    return 1;
}

int verificar_nivel(unsigned nivel_adivinado, unsigned nivel_pokemon){
    if (nivel_adivinado == nivel_pokemon){
        return 0;
    }

    return -1;
}

const char* verificacion_a_string(int resultado_verificacion){
    if (resultado_verificacion == 0){
        return "Hit";
    }

    return "Miss";
}

/* Pruebas */

void puedoCrearYDestruirUnHospital(){
    hospital_t* h=NULL;

    pa2m_afirmar((h=hospital_crear()), "Crear un hospital devuelve un hospital");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital se crea con cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital se crea con cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema(){
    hospital_t* h=NULL;

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital NULL tiene cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital NULL tiene cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon de un hospital NULL resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/archivo_vacio.hospital"), "Puedo leer un archivo vacío");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital vacío tiene cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital vacío tiene tiene cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/un_entrenador.hospital"), "Puedo leer un archivo con un entrenador");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==1, "El hospital tiene 1 entrenador");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==3, "El hospital tiene 3 pokemon");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon)==3, "Recorrer los pokemon resulta en 3 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}

void dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==5, "El hospital tiene 5 entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==24, "El hospital tiene 24 pokemon");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon)==24, "Recorrer los pokemon resulta en 24 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon_hasta_miltank)==14, "Recorrer los pokemon resulta en 14 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}


void dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");
    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer otro archivo con varios entrenadores");
    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un tercer archivo con varios entrenadores");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==15, "El hospital tiene 15 entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==72, "El hospital tiene 72 pokemon");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon)==72, "Recorrer los pokemon resulta en 72 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}

/* Pruebas simulador */
void dadosNulos_NadaPasa(){
    simulador_t* simulador = NULL;

    for (int i = 0; i < 7; i++){
        ResultadoSimulacion resultado = simulador_simular_evento(simulador, (EventoSimulacion)i, NULL);
        pa2m_afirmar(resultado == ErrorSimulacion, "No se pudo simular evento");
    }

    simulador = simulador_crear(NULL);

    pa2m_afirmar(!simulador, "No puedo crear un simulador con un hospital nulo");
}

void buen_uso_simulador(){
    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t* simulador = simulador_crear(hospital);
    pa2m_afirmar(simulador, "Se pudo crear el simulador con el hospital de varios entrenadores");


    DatosDificultad new_diff;
    new_diff.nombre = "Inutil";
    new_diff.calcular_puntaje = calcular_puntaje;
    new_diff.verificar_nivel = verificar_nivel;
    new_diff.verificacion_a_string = verificacion_a_string;
    pa2m_afirmar(simulador_simular_evento(simulador, AgregarDificultad, &new_diff) == ExitoSimulacion, "Se pudo agregar una nueva dificultad");

    int id_new_diff = 3;
    pa2m_afirmar(simulador_simular_evento(simulador, SeleccionarDificultad, &id_new_diff) == ExitoSimulacion, "Se pudo seleccionar la nueva dificultad");


    EstadisticasSimulacion stats1;
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &stats1) == ExitoSimulacion, "Se pudieron obtener estadisticas 1");
    pa2m_afirmar(stats1.cantidad_eventos_simulados == 3, "Se simularon 3 eventos");
    pa2m_afirmar(stats1.entrenadores_totales == 5, "Se tienen 5 entrenadores totales");
    pa2m_afirmar(stats1.pokemon_totales == 24, "Se tienen 24 pokemones totales");
    pa2m_afirmar(stats1.pokemon_en_espera == 0, "No se tienen pokemones en espera");


    pa2m_afirmar(simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ExitoSimulacion, "Se pudo atender al proximo entrenador");

    EstadisticasSimulacion stats2;
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &stats2) == ExitoSimulacion, "Se pudieron obtener estadisticas 2");
    pa2m_afirmar(stats2.cantidad_eventos_simulados == 5, "Se simularon 5 eventos");
    pa2m_afirmar(stats2.pokemon_en_espera == 3, "Se tienen 3 pokemones en espera (uno esta siendo atendido)");
    pa2m_afirmar(stats2.entrenadores_atendidos == 1, "Se atendio 1 entrenador");


    unsigned int vector_niveles_entrenador1[24] = {10, 20, 43, 85};

    Intento adivinanza1;
    adivinanza1.nivel_adivinado = 5;
    pa2m_afirmar(simulador_simular_evento(simulador, AdivinarNivelPokemon, &adivinanza1) == ExitoSimulacion, "Se pudo intentar adivinar el nivel del pokemon atendido");
    pa2m_afirmar(!adivinanza1.es_correcto, "El nivel adivinado no es correcto");

    Intento adivinanza2;
    adivinanza2.nivel_adivinado = vector_niveles_entrenador1[0];
    pa2m_afirmar(simulador_simular_evento(simulador, AdivinarNivelPokemon, &adivinanza2) == ExitoSimulacion, "Se pudo intentar adivinar el nivel del pokemon atendido nuevamente");
    pa2m_afirmar(adivinanza2.es_correcto, "El nivel adivinado ahora si es correcto");

    EstadisticasSimulacion stats3;
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &stats3) == ExitoSimulacion, "Se pudieron obtener estadisticas 3");
    pa2m_afirmar(stats3.cantidad_eventos_simulados == 8, "Se simularon 8 eventos");
    pa2m_afirmar(stats3.pokemon_en_espera == 2, "Se tienen 2 pokemones en espera");
    pa2m_afirmar(stats3.pokemon_atendidos == 1, "Se atendio 1 pokemon");

    for (int i = 1; i < 4; i++){
        Intento adivinanza3;
        adivinanza3.nivel_adivinado = vector_niveles_entrenador1[i];
        pa2m_afirmar(simulador_simular_evento(simulador, AdivinarNivelPokemon, &adivinanza3) == ExitoSimulacion, "Se pudo intentar adivinar otro nivel");
        pa2m_afirmar(adivinanza3.es_correcto, "El nivel adivinado es correcto");
    }

    pa2m_afirmar(simulador_simular_evento(simulador, AdivinarNivelPokemon, &adivinanza1) == ErrorSimulacion, "Intentar adivinar otro nivel devuelve error");

    EstadisticasSimulacion stats4;
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &stats4) == ExitoSimulacion, "Se pudieron obtener estadisticas 4");
    pa2m_afirmar(stats4.cantidad_eventos_simulados == 13, "Se simularon 13 eventos");
    pa2m_afirmar(stats4.pokemon_en_espera == 0, "Se tienen 0 pokemones en espera");
    pa2m_afirmar(stats4.pokemon_atendidos == 4, "Se atendieron 4 pokemones");
    pa2m_afirmar(stats4.puntos == 4, "Tengo 4 puntos como se esperaba");


    pa2m_afirmar(simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ExitoSimulacion, "Se pudo atender al proximo entrenador");
    for (int i = 0; i < 3; i++){
        pa2m_afirmar(simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ExitoSimulacion, "Se pudo atender otro entrenador consecutivo");
    }
    pa2m_afirmar(simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ErrorSimulacion, "No se pudo atender otro entrenador porque no hay");

    EstadisticasSimulacion stats5;
    simulador_simular_evento(simulador, ObtenerEstadisticas, &stats5);
    pa2m_afirmar(stats5.entrenadores_atendidos == 5, "Se atendieron ya los 5 entrenadores");
    pa2m_afirmar(stats5.pokemon_en_espera == 19, "Hay 19 pokemones en espera y uno siendo atendido");


    unsigned int vector_niveles_proximos[20] = {20, 14, 22, 28, 29, 32, 32, 32, 33, 35, 43, 43, 45, 48, 52, 54, 59, 65, 86, 93};
    for (int i = 0; i < 20; i++){
        Intento adivinanza3;
        adivinanza3.nivel_adivinado = vector_niveles_proximos[i];
        simulador_simular_evento(simulador, AdivinarNivelPokemon, &adivinanza3);
    }

    EstadisticasSimulacion stats6;
    simulador_simular_evento(simulador, ObtenerEstadisticas, &stats6);
    pa2m_afirmar(stats6.cantidad_eventos_simulados == 40, "Se simularon 40 eventos");
    pa2m_afirmar(stats6.pokemon_atendidos == 24, "Se atendieron los 24 pokemones");
    pa2m_afirmar(stats6.pokemon_en_espera == 0, "No hay pokemones en espera");
    pa2m_afirmar(stats6.puntos == 24, "Se tienen los 24 puntos que se esperan");


    simulador_simular_evento(simulador, FinalizarSimulacion, NULL);
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &stats1) == ErrorSimulacion, "No se pueden simular mas eventos luego de finalizar la simulacion");


    simulador_destruir(simulador);
}

int main(){

    pa2m_nuevo_grupo("Pruebas de  creación y destrucción");
    puedoCrearYDestruirUnHospital();

    pa2m_nuevo_grupo("Pruebas con NULL");
    dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema();

    pa2m_nuevo_grupo("Pruebas con un archivo vacío");
    dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con un archivo de un entrenador");
    dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con un archivo de varios entrenadores");
    dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con mas de un archivo");
    dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital();

    pa2m_nuevo_grupo("Pruebas de hospital y simulador nulo");
    dadosNulos_NadaPasa();

    pa2m_nuevo_grupo("Pruebas de hospital y simulador no nulo");
    buen_uso_simulador();

    return pa2m_mostrar_reporte();
}
