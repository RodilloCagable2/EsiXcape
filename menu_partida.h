#ifndef MENU_PARTIDA_H_
#define MENU_PARTIDA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jugadores.h"
#include "salas.h"
#include "partida.h"
#include "objetos.h"
#include "conexiones.h"
#include "puzles.h"

//PRECONDICIÓN: Ninguna.
//POSCONDICIÓN: Limpia la consola antes de pintar una nueva pantalla del menú de partida.
static void limpiar_pantalla();

//PRECONDICIÓN: Ninguna.
//POSCONDICIÓN: Pausa la ejecución hasta que el usuario pulse Enter.
static void pausar_pantalla();

//PRECONDICIÓN: s debe ser una cadena válida o NULL.
//POSCONDICIÓN: Devuelve una copia dinámica de la cadena recibida.
static char *duplicar_cadena(const char *);

//PRECONDICIÓN: min debe ser menor o igual que max.
//POSCONDICIÓN: Lee un entero válido dentro del rango indicado.
static int leer_entero_rango(const char *, int, int);

//PRECONDICIÓN: destino debe apuntar a un buffer válido.
//POSCONDICIÓN: Lee una línea de texto eliminando el salto de línea final.
static void leer_texto_simple(const char *, char *, size_t);

//PRECONDICIÓN: salas debe apuntar a una estructura ya cargada.
//POSCONDICIÓN: Devuelve el nombre de la sala o "Desconocida" si no existe.
static const char *nombre_sala_por_id(vector_sala *, int);

//PRECONDICIÓN: p debe apuntar a una partida válida.
//POSCONDICIÓN: Muestra el nombre y la descripción de la sala actual.
static void imprimir_descripcion_sala_actual(partida *);

//PRECONDICIÓN: p debe apuntar a una partida válida.
//POSCONDICIÓN: Muestra la sala actual en la parte superior del menú.
static void imprimir_cabecera_partida(partida *);

//PRECONDICIÓN: p debe apuntar a una partida válida e id_obj debe ser una cadena válida.
//POSCONDICIÓN: Devuelve el índice del objeto dentro de la partida o -1 si no existe.
static int buscar_indice_objeto_partida(partida *, const char *);

//PRECONDICIÓN: p debe apuntar a una partida válida e id_conexion debe ser una cadena válida.
//POSCONDICIÓN: Devuelve el índice del estado de esa conexión dentro de la partida o -1 si no existe.
static int buscar_indice_conexion_estado(partida *, const char *);

//PRECONDICIÓN: p debe apuntar a una partida válida e id_puzle debe ser una cadena válida.
//POSCONDICIÓN: Devuelve el índice del puzle dentro del estado de partida o -1 si no existe.
static int buscar_indice_puzle_estado(partida *, const char *);

//PRECONDICIÓN: p debe apuntar a una partida válida.
//POSCONDICIÓN: Limpia pantalla y muestra los objetos de la sala y las salidas disponibles.
static void examinar(partida *);

//PRECONDICIÓN: p debe apuntar a una partida válida.
//POSCONDICIÓN: Muestra las salas a las que se puede ir desde la actual y permite volver a la sala anterior aunque no exista una conexión explícita de vuelta.
static void entrar_en_otra_sala(partida *);

//PRECONDICIÓN: La partida y el jugador deben ser válidos.
//POSCONDICIÓN: Permite coger un objeto existente en la sala y moverlo al inventario.
static void coger_objeto(partida *, jug_vect *, int);

//PRECONDICIÓN: La partida y el jugador deben ser válidos.
//POSCONDICIÓN: Permite mover un objeto del inventario del jugador a la sala actual.
static void soltar_objeto(partida *, jug_vect *, int);

//PRECONDICIÓN: j debe ser un vector válido y el índice del jugador debe existir.
//POSCONDICIÓN: Limpia pantalla y muestra todos los objetos del inventario del jugador.
static void mostrar_inventario(jug_vect *, int);

//PRECONDICIÓN: La partida y el jugador deben ser válidos.
//POSCONDICIÓN: Permite usar un objeto del inventario para abrir conexiones adyacentes de la sala actual.
static void usar_objeto(partida *, jug_vect *, int);

//PRECONDICIÓN: p debe apuntar a una partida válida.
//POSCONDICIÓN: Permite resolver el puzle de la sala actual y desbloquear las conexiones asociadas.
static void resolver_puzle(partida *);

//PRECONDICIÓN: Los parámetros deben referirse a estructuras válidas ya cargadas.
//POSCONDICIÓN: Guarda todas las partidas y sincroniza el inventario del jugador activo.
static void guardar_partida_menu(partida *, int, jug_vect *, int, int);

//PRECONDICIÓN: Entran el vector de partidas, el número de partidas, el vector de jugadores, el índice de la partida activa y el índice del jugador autenticado.
//POSCONDICIÓN: Ejecuta el menú de partida permitiendo realizar todas las acciones del juego. Devuelve 0 si el jugador vuelve al menú principal y -1 si decide salir del programa.
int mostrar_partida(partida *, int, jug_vect *, int, int);

#endif
