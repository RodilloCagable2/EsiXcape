#ifndef MENU_H_
#define MENU_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "jugadores.h"

//---> FUNCIONES: <---

static void limpiar_buffer_entrada(void);

static int buscar_usuario(jug_vect *, const char *);

static int obtener_siguiente_id(jug_vect *);

static int registrar_jugador(jug_vect *, const char *, const char *, const char *);

static int autenticar_o_registrar(jug_vect *);

//PRECONDICIÓN: Las estructuras de jugadores deben de estar inicializadas antes de ser pasadas por referencia a la función
//POSCONDICIÓN: Se ejecuta un menú principal que te deja moverte entre tres opciones posibles: Nueva partida, Cargar partida, Salir
int menu_principal(jug_vect *);

#endif



