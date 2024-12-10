#include "hospital.h"
#include "structhospital.h"
#include "split.h"

#include <string.h>
#include <stdio.h>


/* Funciones propias */

/* Toma los objetos pasados por parametro como
   si fuesen pokemon y compara sus nombres.*/
int comparador_nombre_pokemon(void* _poke1, void* _poke2){
    pokemon_t* poke1 = _poke1;
    pokemon_t* poke2 = _poke2;

    return strcmp(poke1->nombre, poke2->nombre);
}

/* Toma los objetos pasados por parametro como
   si fuesen pokemon y compara sus niveles.*/
int comparador_nivel_pokemon(void* _poke1, void* _poke2){
    pokemon_t* poke1 = _poke1;
    pokemon_t* poke2 = _poke2;

    return (int)poke1->nivel - (int)poke2->nivel;
}

/* Libera toda la memoria del objeto pasado por
   parametro, interpretandolo como un pokemon_t.

   No libera si el objeto es NULL.*/
void destruir_pokemon(void* _poke){
    if (!_poke){
        return;
    }

    pokemon_t* poke = _poke;
    free(poke->nombre);
    free(poke);
}

/* Funcion adaptada para funcionar con el iterador de lista.
   Libera la memoria del entrenador, al igual que destruir
   sus pokemon junto a su respectivo ABB.

   No libera si el entrenador es NULO.*/
bool destruir_entrenador_para_lista(void* _entrenador, void* nulo){
    if (!_entrenador){
        return false;
    }

    entrenador_t* entrenador = _entrenador;   
    abb_destruir_todo(entrenador->pokemones, destruir_pokemon);
    free(entrenador->nombre);
    free(entrenador);

    return true;
}

/* Recibe un archivo no nulo y abierto correctamente.
   Devuelve el string hasta el siguiente salto de linea o el fin del archivo.
   Devuelve NULL si no se pudo reservar memoria o si no se leyo ningun byte ->
   A. K. A. ya se recorrio todo el archivo.*/
char* obtener_siguiente_linea(FILE* archivo){
    size_t bytes_leidos = 0;
    size_t longitud = 128;
    char* linea = (char*)malloc(longitud);
    if (!linea){
        return NULL;
    }

    while (fgets(linea + bytes_leidos, (int)(longitud - bytes_leidos), archivo)){
        size_t chars_leidos = strlen(linea + bytes_leidos);
        
        if ((chars_leidos > 0) && (linea[bytes_leidos + chars_leidos - 1] == '\n')){
            linea[bytes_leidos + chars_leidos - 1] = 0;
            return linea;

        } else {
            char* linea_aux = (char*)realloc(linea, longitud * 2);
            if (!linea_aux){
                free(linea);
                return NULL;
            }
            linea = linea_aux;
            longitud *= 2;
        }
        bytes_leidos += chars_leidos;
    }

    if (bytes_leidos == 0){
        free(linea);
        return NULL;
    }

    return linea;
}

/* Devuelve la cantidad de pokemones (siendo esto la cantidad de elementos en el split sin contar
   el id del entrenador, el nombre del entrenador y los niveles de los pokemon). */
size_t cantidad_pokemones(char** linea_splitted){
    size_t contador = 0;
    while(linea_splitted[contador]){
        contador++;
    }
    return (contador - 2) / 2;
}

/* Inserta a la lista de entrenadores un nuevo entrenador,
   junto a sus campos correctamente asignados. Crea un ABB
   para los pokemon de este entrenador y los inserta en el,
   segun el orden del split especificado en la consigna del
   hospital pokemon, siendo esta:

   ID;NOMBRE_ENTRENADOR;NOMBRE_POKEMON;NIVEL_POKEMON;...

   No funciona si el hospital o el split son nulos.*/
void splitted_a_hospital(hospital_t* hospital, char** splitted){
    if (!hospital || !splitted){
        return;
    }

    entrenador_t* nuevo_entrenador = calloc(1, sizeof(entrenador_t));
    if (!nuevo_entrenador){
        return;
    }

    nuevo_entrenador->id = atoi(splitted[0]);
    nuevo_entrenador->nombre = splitted[1];
    nuevo_entrenador->pokemones = abb_crear(comparador_nivel_pokemon);
    if (!nuevo_entrenador->pokemones){
        free(nuevo_entrenador->nombre);
        free(nuevo_entrenador);
    }

    size_t cant_pokemon_de_entrenador = cantidad_pokemones(splitted);
    for (size_t i = 0; i < cant_pokemon_de_entrenador; i++){
        pokemon_t* nuevo_pokemon = calloc(1, sizeof(pokemon_t));
        nuevo_pokemon->nombre = splitted[2 + 2*i];
        nuevo_pokemon->nivel = (size_t)atoi(splitted[3 + 2*i]);
        nuevo_pokemon->entrenador = nuevo_entrenador;

        abb_insertar(nuevo_entrenador->pokemones, (void*)nuevo_pokemon);
        hospital->cantidad_pokemones++;
    }

    lista_insertar(hospital->lista_entrenadores, (void*)nuevo_entrenador);
}

/* Libera la memoria del ID y el Nivel de los pokemon dentro del split.
   Libera la memoria del split.
   
   No libera si el split es NULL.*/
void liberar_split_hospital(char** splitted){
    size_t cant_pokemones_a_liberar = cantidad_pokemones(splitted);
    free(splitted[0]);
    for (size_t i = 0 ; i < cant_pokemones_a_liberar ; i++){
        free(splitted[3 + 2*i]);
    }
    free(splitted);
}

/* Funcion adaptada para funcionar con el iterador de ABB.
   Inserta el objeto del primer parametro, asumido un pokemon
   en el presunto ABB del segundo parametro.

   Devuelve siempre true a menos que alguno de los dos 
   parametros es nulo. */
bool insertar_pokemon_en_abb(void* _poke, void* _abb_pokemones){
    if (!_poke || !_abb_pokemones){
        return false;
    }

    abb_insertar((abb_t*)_abb_pokemones, _poke);
    return true;
}

/* Funcion adaprada para funcionar con el iterador de ABB.
   Inserta cada pokemon del ABB del entrenador en el ABB
   pasado como segundo parametro.

   Devuelve siempre true a menos que alguno de los dos
   parametros sea nulo.*/
bool entrenador_a_abb_pokemon(void* _entrenador, void* _abb_pokemones){
    if (!_entrenador || !_abb_pokemones){
        return false;
    }

    entrenador_t* entrenador = _entrenador;
    abb_con_cada_elemento(entrenador->pokemones, PREORDEN, insertar_pokemon_en_abb, _abb_pokemones);
    
    return true;
}

/* Crea un ABB con todos los pokemones de la lista de entrenadores
   pasada por parametro, usando como comparacion sus nombres.

   Devuelve el ABB con los pokemones en el orden definido o
   devuelve NULL si algo fallo.

   El usuario ha de destruir el ABB por su cuenta. Se recomienda
   no destruir los pokemon.*/
abb_t* dump_lista_a_abb(lista_t* lista_entrenadores){
    if (!lista_entrenadores){
        return NULL;
    }

    abb_t* abb_pokemones = abb_crear(comparador_nombre_pokemon);
    if (!abb_pokemones){
        return NULL;
    }

    lista_con_cada_elemento(lista_entrenadores, entrenador_a_abb_pokemon, (void*)abb_pokemones);

    return abb_pokemones;
}


/* Funciones del TP */

hospital_t* hospital_crear(){
    hospital_t* hospital = calloc(1, sizeof(hospital_t));
    if (!hospital){
        return NULL;
    }

    hospital->lista_entrenadores = lista_crear();
    if (!hospital->lista_entrenadores){
        free(hospital);
        return NULL;
    }

    return hospital;
}

bool hospital_leer_archivo(hospital_t* hospital, const char* nombre_archivo){
    if (!hospital || !nombre_archivo){
        return false;
    }

    FILE* stream = fopen(nombre_archivo, "r");
    if (!stream){
        return false;
    }

    char* linea_actual = obtener_siguiente_linea(stream);

    while (linea_actual){
        char** splitted = split(linea_actual, ';');
        if (!splitted){
            fclose(stream);
            return false;
        }

        splitted_a_hospital(hospital, splitted);
        
        free(linea_actual);
        liberar_split_hospital(splitted);
        linea_actual = obtener_siguiente_linea(stream);
    }

    fclose(stream);
    free(linea_actual);

    return true;
}

size_t hospital_cantidad_entrenadores(hospital_t* hospital){
    if (!hospital){
        return 0;
    }

    return lista_tamanio(hospital->lista_entrenadores);
}

size_t hospital_cantidad_pokemon(hospital_t* hospital){
    if (!hospital){
        return 0;
    }

    return hospital->cantidad_pokemones;
}

bool hospital_guardar_archivo(hospital_t* hospital, const char* nombre_archivo){
    return false;
}

size_t hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p)){
    if (!hospital || !funcion){
        return 0;
    }

    abb_t* abb_aux = dump_lista_a_abb(hospital->lista_entrenadores);
    if (!abb_aux){
        return 0;
    }

    size_t cant_pokemones = abb_tamanio(abb_aux);
    pokemon_t** vector_pokemones = calloc(cant_pokemones, sizeof(pokemon_t*));
    if (!vector_pokemones){
        abb_destruir(abb_aux);
        return 0;
    }
    abb_recorrer(abb_aux, INORDEN, (void**)vector_pokemones, cant_pokemones);

    size_t contador = 0;
    bool continuar = true;

    while (continuar && (contador < cant_pokemones)){
        continuar = funcion(vector_pokemones[contador]);
        contador++;
    }

    abb_destruir(abb_aux);
    free(vector_pokemones);
    return contador;
}

void hospital_destruir(hospital_t* hospital){
    if (!hospital){
        return;
    }

    lista_con_cada_elemento(hospital->lista_entrenadores, destruir_entrenador_para_lista, NULL);
    lista_destruir(hospital->lista_entrenadores);
    free(hospital);
}

size_t pokemon_nivel(pokemon_t* pokemon){
    if (!pokemon){
        return 0;
    }

    return pokemon->nivel;
}

const char* pokemon_nombre(pokemon_t* pokemon){
    if (!pokemon){
        return NULL;
    }

    return pokemon->nombre;
}

entrenador_t* pokemon_entrenador(pokemon_t* pokemon){
    if (!pokemon){
        return NULL;
    }

    return pokemon->entrenador;
}
