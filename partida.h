#ifndef PARTIDA_H
#define PARTIDA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define max_id_jugador 3
#define max_id_sala 3
#define max_id_objeto 5
#define max_localizacion 11
#define max_id_conexion 4
#define max_estado_conexion 11
#define max_id_puzle 4
#define max_estado_puzle 11

typedef struct{
    char id_objeto[max_id_objeto];
    char localizacion[max_localizacion];
} objeto_partida;

typedef struct{
    char id_conexion[max_id_conexion];
    char estado[max_estado_conexion];
} conexion_partida;

typedef struct{
    char id_puzle[max_id_puzle];
    char estado[max_estado_puzle];
    struct puzle_partida *siguiente;
} puzle_partida;

typedef struct{
    char id_jugador[max_id_jugador];
    char id_sala_actual[max_id_sala];

    objeto_partida *objetos;

    conexion_partida *conexiones;

    puzle_partida *puzles;
} partida;

//cabecera: cargar_partida(partida **p)
//precondición: Entra un puntero a puntero de partida.
//postcondición: Carga los datos del fichero "partida.txt" en memoria dinámica, creando un array de partidas. Devuelve el número de partidas cargadas.
int cargar_partida(partida **p);

//cabecera: guardar_partida(partida *p)
//precondición: Entra una estructura partida con datos válidos. Cada jugador solo puede tener una partida guardada, pero puede haber varios jugadores con su partida.
//postcondición: Guarda el estado actual de la partida en el fichero "partida.txt".
void guardar_partida(partida *p);

//cabecera: mostrar_partida(partida *p)
//precondición: Entra una estructura partida cargada.
//postcondición: Muestra por pantalla todos los datos de la partida.
void mostrar_partida(partida *p);

//cabecera: eliminar_partida(partida *p)
//precondición: Entra una estructura partida inicializada.
//postcondición: Libera la memoria dinámica asociada a la partida.
void eliminar_partida(partida *p);

//cabecera: buscar_objeto_partida(partida *p, char *id_objeto)
//precondición: Entra una partida y un id de objeto.
//postcondición: Devuelve el índice del objeto o -1 si no existe.
//int buscar_objeto_partida(partida *p, char *id_objeto);

//cabecera: buscar_conexion_partida(partida *p, char *id_conexion)
//precondición: Entra una partida y un id de conexión.
//postcondición: Devuelve el índice de la conexión o -1 si no existe.
//int buscar_conexion_partida(partida *p, char *id_conexion);

//cabecera: buscar_puzle_partida(partida *p, char *id_puzle)
//precondición: Entra una partida y un id de puzle.
//postcondición: Devuelve el índice del puzle o -1 si no existe.
//int buscar_puzle_partida(partida *p, char *id_puzle);

//cabecera: limpiar_cadena_partida(char *s)
//precondición: Entra una cadena.
//postcondición: Elimina los caracteres '\n' y '\r'.
void limpiar_cadena_partida(char *s);

//cabecera: existe_jugador_partida(partida *p, int num_partidas, char *id_jugador)
//precondición: Entra un array de partidas, el número de partidas y un id de jugador.
//postcondición: Devuelve 1 si el jugador ya tiene una partida guardada, 0 si no.
int existe_jugador_partida(partida *p, int num_partidas, char *id_jugador);

//cabecera: escribir_una_partida(FILE *f, partida *p)
//precondición: Entra un puntero a archivo abierto para escritura y una partida con datos válidos.
//postcondición: Escribe la partida en el formato adecuado en el archivo.
void escribir_una_partida(FILE *f, partida *p);

#endif