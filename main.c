#include "src/hospital.h"
#include "src/simulador.h"
#include "src/colores.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define NOMBRE_DIFICIL_MAIN "Estilo Rammus"
#define NOMBRE_FACIL_MAIN "Facilisisimo"

#define MAX_NIVEL_POSIBLE_PARA_FACIL 9999
#define MIN_NIVEL_POSIBLE_PARA_FACIL -999
#define ID_DIFICULTAD_FACIL 4

#define FINALIZARSIMULACION 'q'
#define OBTENERESTADISTICAS 'e'
#define ATENDERPROXIMOENTRENADOR 'p'
#define OBTENERINFORMACIONPOKEMONENTRATAMIENTO 'i'
#define ADIVINARNIVELPOKEMON 'a'
#define AGREGARDIFICULTAD 'n'
#define SELECCIONARDIFICULTAD 'd'
#define OBTENERINFORMACIONDIFICULTAD 'o'
#define CAMBIARDIFICULTAD 'c'
#define MENSAJECOMANDOMALO "\n\033[91mNo es un comando valido. \n\033[0m"
#define AYUDAS "\n\033[33mUsa para... \nObtener estadisticas: E \nAtender proximo entrenador: P \nObtener info pokemon en tratamiento: I \nAdivinar el nivel: A \nObtener info dificultad por ID: O \nCambiar dificultad: C \nFinalizar simulacion: Q \n\n\033[0m"
#define TERMINAL_LIMPIAR "clear"

/* Funciones modificables para poder agregar con agregar_dificultad() */
unsigned calcular_puntaje_dificil(unsigned cantidad_intentos){
    return 10;
}

int verificar_nivel_dificil(unsigned nivel_adivinado, unsigned nivel_pokemon){
    if (nivel_adivinado == nivel_pokemon){
        return 0;
    }

    return -1;
}

const char* verificacion_a_string_dificil(int resultado_verificacion){
    if (resultado_verificacion == 0){
        return "Ok";
    }

    return "No ok";
}

unsigned calcular_puntaje_facil(unsigned cantidad_intentos){
    if (cantidad_intentos == 1){
        return 1000;
    } else {
        return 1000 - (2 * cantidad_intentos);
    }
}

int verificar_nivel_facil(unsigned nivel_adivinado, unsigned nivel_pokemon){
    return (int)nivel_adivinado - (int)nivel_pokemon;
}

const char* verificacion_a_string_facil(int resultado_verificacion){
    if (resultado_verificacion > MAX_NIVEL_POSIBLE_PARA_FACIL){
        return "Estas muy por arriba del nivel correcto para darte una pista.";
    } else if (resultado_verificacion < MIN_NIVEL_POSIBLE_PARA_FACIL){
        return "Estas muy por abajo del nivel correcto para darte una pista.";
    } else if (resultado_verificacion == 0){
        return "Correcto!!!";
    } else {
        char* mensaje = calloc(35, sizeof(char));
        sprintf(mensaje, "Le erraste por %d niveles", resultado_verificacion);

        return mensaje;
    }

}


/* Funciones utiles */
char leer_comando(){
    char linea[100];
    char* leido;
    leido = fgets(linea, 100, stdin);
    if(!leido)
        return 0;
    while(*leido == ' ')
        leido++;
    return (char)tolower(*leido);
}

char leer_caracter(){
    char obtenido = '\0';
    scanf(" %c%*[^\n]", &obtenido);
    return (char)tolower(obtenido);
}

void limpiar_terminal(){
    char cmd[6];
    strcpy(cmd, TERMINAL_LIMPIAR);
    system(cmd);
}

/* Devuelve true si la dificultad actual del simulador
   es la "Facil del main". */
bool es_dificultad_facil(simulador_t* simulador){
    InformacionDificultad info;
    info.id = ID_DIFICULTAD_FACIL;

    simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info);

    if (!info.nombre_dificultad || (info.id == -1)){
        printf("\n%s%sAlgo paso al chequear si la dificultad facil del main esta en uso. \n%s", LETRA_NEGRITA, COLOR_ROJO, COLOR_BASE);
        return false;
    }

    if (strcmp(info.nombre_dificultad, NOMBRE_FACIL_MAIN) == 0){
        if (info.en_uso){
            return true;
        }
    } else {
        printf("\n%s%sEl nombre de la dificultad esta mal. \n%s", LETRA_NEGRITA, COLOR_ROJO, COLOR_BASE);
    }

    return false;
}


/* Funciones de simulacion */
void terminar_simulacion(simulador_t* simulador){
    simulador_simular_evento(simulador, FinalizarSimulacion, NULL);
}

void mostrar_estadisticas(simulador_t* simulador){
    EstadisticasSimulacion estadisticas;

    ResultadoSimulacion resultado = simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);
    if (resultado == ErrorSimulacion){
        printf("\n%s%sSimulacion fallida! \n\n%s", LETRA_NEGRITA, COLOR_ROJO, COLOR_BASE);
    }

    printf("\n%sEntrenadores atendidos: %i \nEntrenadores totales; %i \n%sPokemones atendidos: %i \nPokemones en espera: %i \nPokemones totales: %i \n%sPuntos: %i \nCantidad de eventos simulados: %i \n\n%s", 
        COLOR_VIOLETA, estadisticas.entrenadores_atendidos, estadisticas.entrenadores_totales, 
        COLOR_LILA, estadisticas.pokemon_atendidos, estadisticas.pokemon_en_espera, estadisticas.pokemon_totales, 
        COLOR_CELESTE, estadisticas.puntos, estadisticas.cantidad_eventos_simulados, COLOR_BASE);
}

void atender_proximo_entrenador(simulador_t* simulador){
    ResultadoSimulacion resultado = simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);
    if (resultado == ErrorSimulacion){
        printf("\n%s%sSimulacion fallida! \n\n%s", LETRA_NEGRITA, COLOR_ROJO, COLOR_BASE);
        return;
    }

    printf("\n%s%sProximo entrenador atendido! \n\n%s", LETRA_NEGRITA, COLOR_VERDE, COLOR_BASE);
}

void obtener_informacion_pokemon_en_tratamiento(simulador_t* simulador){
    InformacionPokemon info;

    ResultadoSimulacion resultado = simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &info);
    if (resultado == ErrorSimulacion){
        printf("\n%s%sSimulacion fallida! \n\n%s", LETRA_NEGRITA, COLOR_ROJO, COLOR_BASE);
        return;
    }

    printf("\n%s%sDatos pokemon en tratamiento...%s \n%sNombre pokemon: %s \n%sNombre entrenador: %s \n\n", LETRA_NEGRITA, COLOR_VERDE, COLOR_BASE, COLOR_VIOLETA, info.nombre_pokemon, COLOR_LILA, info.nombre_entrenador);
}

void adivinar_nivel_pokemon(simulador_t* simulador){
    Intento adivinanza;
    printf("\n%s%sQue nivel seria el pokemon en tratamiento? %s", LETRA_NEGRITA, COLOR_AZUL, COLOR_BASE);
    scanf(" %i", &adivinanza.nivel_adivinado);

    ResultadoSimulacion resultado = simulador_simular_evento(simulador, AdivinarNivelPokemon, &adivinanza);
    if (resultado == ErrorSimulacion){
        printf("\n%s%sSimulacion fallida! \n\n%s", LETRA_NEGRITA, COLOR_ROJO, COLOR_BASE);
        return;
    }

    char* correctisidad = NULL;
    if (adivinanza.es_correcto){
        correctisidad = "Si";
    } else {
        correctisidad = "No";
    }

    printf("\n%sNivel adivinado: %i \n%sEs acertado? %s", COLOR_VIOLETA, adivinanza.nivel_adivinado, COLOR_LILA, correctisidad);
    if (adivinanza.resultado_string){
        printf("\n%s%s \n\n", COLOR_CELESTE, adivinanza.resultado_string);

        if (es_dificultad_facil(simulador)){
            char* mensaje = (char*)adivinanza.resultado_string;
            if ((!adivinanza.es_correcto) && (strlen(mensaje) <= 35)){
                free(mensaje);
            }
        }
    } else {
        printf("\n\n");
    }
}

void agregar_dificultad(simulador_t* simulador, const char* nombre, unsigned (*calcular_puntaje)(unsigned), int (*verificar_nivel)(unsigned, unsigned), const char* (*verificacion_a_string)(int)){
    if (!simulador || !nombre || !calcular_puntaje || !verificacion_a_string){
        printf("\n%s%sNueva dificultad no agregada. \n\n%s", LETRA_NEGRITA, COLOR_ROJO, COLOR_BASE);
        return;
    }

    DatosDificultad nueva;
    nueva.nombre = nombre;
    nueva.calcular_puntaje = calcular_puntaje;
    nueva.verificar_nivel = verificar_nivel;
    nueva.verificacion_a_string = verificacion_a_string;

    ResultadoSimulacion resultado = simulador_simular_evento(simulador, AgregarDificultad, &nueva);

    if (resultado == ExitoSimulacion){
        printf("\n%s%sNueva dificultad agregada. \n\n%s", LETRA_NEGRITA, COLOR_VERDE, COLOR_BASE);
    } else {
        printf("\n%s%sNueva dificultad no agregada. \n\n%s", LETRA_NEGRITA, COLOR_ROJO, COLOR_BASE);
    }
}

void seleccionar_dificultad(simulador_t* simulador){
    int id = -1;
    printf("\n%s%sInserte id de la dificultad deseada: %s", LETRA_NEGRITA, COLOR_AZUL, COLOR_BASE);
    scanf(" %i", &id);

    ResultadoSimulacion resultado = simulador_simular_evento(simulador, SeleccionarDificultad, &id);
    if (resultado == ErrorSimulacion){
        printf("\n%s%sSimulacion fallida! \n\n%s", LETRA_NEGRITA, COLOR_ROJO, COLOR_BASE);
    } else{
        printf("\n%s%sDificultad seleccionada correctamente. \n\n%s", LETRA_NEGRITA, COLOR_VERDE, COLOR_BASE);
    }
}

void obterner_informacion_dificultad(simulador_t* simulador){
    int id_buscado = -1;
    printf("\n%s%sInserta el Id de la dificultad la cual quieras obtener informacion: %s", LETRA_NEGRITA, COLOR_AZUL, COLOR_BASE);
    scanf(" %i", &id_buscado);

    InformacionDificultad info;
    info.id = id_buscado;

    ResultadoSimulacion resultado = simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info);
    if (resultado == ErrorSimulacion){
        printf("\n%s%sSimulacion fallida! \n\n%s", LETRA_NEGRITA, COLOR_ROJO, COLOR_BASE);
        return;
    }

    if (!info.nombre_dificultad){
        printf("\n%s%sNo existe una dificultad con el Id insertado. \n\n%s", LETRA_NEGRITA, COLOR_ROJO, COLOR_BASE);
        return;
    }

    char* en_uso = NULL;
    if (info.en_uso){
        en_uso = "si";
    } else {
        en_uso = "no";
    }

    printf("\n%s%sInformacion de dificultad buscada:%s \n%sNombre: %s \n%sEsta en uso: %s \n%sId: %i \n\n", 
            LETRA_NEGRITA, COLOR_VERDE, COLOR_BASE,
            COLOR_VIOLETA ,info.nombre_dificultad,
            COLOR_LILA, en_uso,
            COLOR_CELESTE, info.id);
}


/* Funciones de comandos exclusivamente */
void informar_error_comando(){
    printf(MENSAJECOMANDOMALO);
    printf(AYUDAS);
}

void cambiar_dificultad(simulador_t* simulador){
    printf("\n%s%sDificultades disponibles: \n%s", LETRA_NEGRITA, COLOR_LILA, COLOR_BASE);

    InformacionDificultad info;
    info.id = 0;
    while (info.id != -1){
        simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info);
        
        if (info.id != -1){
            printf("\n%s%s- ID: %i - %s %s", LETRA_NEGRITA, COLOR_CELESTE, info.id, info.nombre_dificultad, COLOR_BASE);
            info.id++;
        }
    }

    int id = -1;
    printf("\n\n%s%sInserte ID de la dificultad deseada: %s", LETRA_NEGRITA, COLOR_AZUL, COLOR_BASE);
    scanf(" %i", &id);

    ResultadoSimulacion resultado = simulador_simular_evento(simulador, SeleccionarDificultad, &id);
    if (resultado == ErrorSimulacion){
        printf("\n%s%sSimulacion fallida! \n\n%s", LETRA_NEGRITA, COLOR_ROJO, COLOR_BASE);
    } else{
        printf("\n%s%sDificultad seleccionada correctamente. \n\n%s", LETRA_NEGRITA, COLOR_VERDE, COLOR_BASE);
    }

}



void ejecutar_comando(simulador_t* simulador, char comando){
    switch (comando) {
        case FINALIZARSIMULACION:
            printf("\n%s%sResultados finales... \n%s", LETRA_NEGRITA, COLOR_VERDE, COLOR_BASE);
            mostrar_estadisticas(simulador);
            terminar_simulacion(simulador);
            break;
        case OBTENERESTADISTICAS:
            mostrar_estadisticas(simulador);
            break;
        case ATENDERPROXIMOENTRENADOR:
            atender_proximo_entrenador(simulador);
            break;
        case OBTENERINFORMACIONPOKEMONENTRATAMIENTO:
            obtener_informacion_pokemon_en_tratamiento(simulador);
            break;
        case ADIVINARNIVELPOKEMON:
            adivinar_nivel_pokemon(simulador);
            break;
        case OBTENERINFORMACIONDIFICULTAD:
            obterner_informacion_dificultad(simulador);
            break;
        case CAMBIARDIFICULTAD:
            cambiar_dificultad(simulador);
            break;
        default:
            informar_error_comando();
            break;
    }
}

int main() {
    limpiar_terminal();
    printf("%sSecuestrando pokemones... \nInyectandoles suplementos ilegales para amplificar sus capacidades de pelea... \nObligandolos a pelear entre ellos para obtener experiencia en el ambito de las peleas legales de especies exoticas... \n\n%s", COLOR_AMARILLO, COLOR_BASE);
    printf("%sCargando simulacion... \n%s%sSimulacion cargada! \n\n%s", COLOR_AMARILLO, LETRA_NEGRITA, COLOR_VERDE, COLOR_BASE);

    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t* simulador = simulador_crear(hospital);
    
    agregar_dificultad(simulador, NOMBRE_DIFICIL_MAIN, calcular_puntaje_dificil, verificar_nivel_dificil, verificacion_a_string_dificil);
    agregar_dificultad(simulador, NOMBRE_FACIL_MAIN, calcular_puntaje_facil, verificar_nivel_facil, verificacion_a_string_facil);

    printf(AYUDAS);
    char comando = '\0';
    do{
        printf("%s%s>Inserte comando: %s", LETRA_NEGRITA, COLOR_AZUL, COLOR_BASE);
        comando = leer_caracter();
        ejecutar_comando(simulador, comando);
    }while(comando != FINALIZARSIMULACION);

    simulador_destruir(simulador);

    return 0;
}
