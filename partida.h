#ifndef PARTIDA_H
#define PARTIDA_H

#include "jugadores.h"
#include "conexiones.h"
#include "objetos.h"
#include "puzles.h"
#include "salas.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
} puzle_partida;

typedef struct{
    int id_jugador;          		// Identificador real del jugador dueño de la partida
    int id_sala_actual;      		// Sala en la que se encuentra el jugador
    int id_sala_anterior;    		// Última sala visitada, para permitir volver atrás
    objeto_partida *objetos; 		// Estado dinámico de los objetos de la partida
    conexion_partida *conexiones; 	// Estado dinámico de las conexiones de la partida
    puzle_partida *puzles;   		// Estado dinámico de los puzles de la partida
} partida;

//PRECONDICIÓN: p debe apuntar a un array de puzles terminado en centinela.
//POSCONDICIÓN: Devuelve el número de puzles existentes.
static int contar_puzles_array(puzle *);

//PRECONDICIÓN: salas debe apuntar a una estructura de salas ya cargada.
//POSCONDICIÓN: Devuelve el id de la sala marcada como INICIAL. Si no existe, devuelve 1.
static int buscar_sala_inicial(vector_sala *);

//PRECONDICIÓN: destino debe ser un puntero válido.
//POSCONDICIÓN: Copia en memoria el estado base del juego para ese jugador: sala inicial, conexiones, objetos y puzles en su estado inicial. Devuelve 1 si todo va bien y 0 en caso de error.
static int inicializar_partida_base(partida *, int);

//PRECONDICIÓN: valor debe contener una línea del tipo "OB01-Inventario" o "OB01-03".
//POSCONDICIÓN: Actualiza la localización del objeto correspondiente dentro de la partida.
static void aplicar_linea_objeto(partida *, char *);

//PRECONDICIÓN: valor debe contener una línea del tipo "C01-Activa".
//POSCONDICIÓN: Actualiza el estado de la conexión correspondiente.
static void aplicar_linea_conexion(partida *, char *);

//PRECONDICIÓN: valor debe contener una línea del tipo "P01-Resuelto".
//POSCONDICIÓN: Actualiza el estado del puzle correspondiente.
static void aplicar_linea_puzle(partida *, char *);

//PRECONDICIÓN: Entra una cadena válida.
//POSCONDICIÓN: Elimina los caracteres '\n' y '\r' de la cadena.
void limpiar_cadena_partida(char *);

//PRECONDICIÓN: Entra el vector de partidas y el identificador real del jugador.
//POSCONDICIÓN: Devuelve el índice de la partida del jugador o -1 si no existe.
int buscar_indice_partida(partida *, int, int);

//PRECONDICIÓN: Entran un vector de partidas y el identificador real del jugador.
//POSCONDICIÓN: Devuelve 1 si el jugador tiene partida guardada y 0 en caso contrario.
int existe_jugador_partida(partida *, int, int);

//PRECONDICIÓN: Entra un puntero a puntero de partida.
//POSCONDICIÓN: Carga los datos del fichero "partida.txt" en memoria dinámica y devuelve el número de partidas cargadas.
int cargar_partida(partida **);

//PRECONDICIÓN: Entran un fichero abierto para escritura y una partida válida.
//POSCONDICIÓN: Escribe la partida en el formato correcto de Partida.txt.
void escribir_una_partida(FILE *, partida *);

//PRECONDICIÓN: Entran todas las partidas cargadas en memoria.
//POSCONDICIÓN: Guarda en partida.txt el estado actual de todas las partidas.
void guardar_partida(partida *, int, jugadores);

//PRECONDICIÓN: Entran el vector de partidas y el índice del jugador autenticado.
//POSCONDICIÓN: Crea una partida nueva desde el estado base del juego. Si el jugador ya tenía partida, la reinicia desde la sala inicial. Devuelve el índice de la partida creada/reiniciada.
int crear_partida_nueva(partida **, int *, jug_vect *, int);

//PRECONDICIÓN: Entra una partida válida y el índice del jugador autenticado.
//POSCONDICIÓN: Actualiza el campo Id_obj del jugador a partir del estado real de la partida.
void sincronizar_inventario_desde_partida(partida *, jug_vect *, int);

//PRECONDICIÓN: Entra una partida válida.
//POSCONDICIÓN: Libera la memoria dinámica reservada para esa partida.
void liberar_partida_individual(partida *);

//PRECONDICIÓN: Entra un vector dinámico de partidas.
//POSCONDICIÓN: Libera la memoria dinámica de todas las partidas y del propio vector.
void liberar_partidas(partida *, int);

//PRECONDICIÓN: Entra el vector dinámico de partidas y el identificador real del jugador.
//POSCONDICIÓN: Elimina la partida de ese jugador tanto de memoria como del fichero.
void eliminar_partida(partida **, int *, int);

#endif
