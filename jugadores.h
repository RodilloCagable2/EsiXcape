#ifndef JUGADORES_H_
#define JUGADORES_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//---> ESTRUCTURAS: <---

typedef struct {
	int Id_jugador;			//Identificador del usuario
	char Nomb_jugador[21];	//Nombre completo del usuario
	char Jugador[11];		//Nombre de jugador para acceder al sistema
	char Contrasena[9];		//Contraseña para acceder al sistema
	char *Id_obj;			//Inventario de los objetos del jugador
}jugadores;

typedef struct {
	jugadores *jug;
	int num_jug;
}jug_vect;

//---> FUNCIONES: <---

//PRECONDICIÓN: Ninguna
//POSCONDICIÓN: Devuelve 0 si la carga de datos fue exitosa, 1 en caso de error. La estructura apuntada por j se actualiza con los datos cargados. 
void cargar_jugadores(jug_vect *);

//PRECONDICIÓN: j debe ser un puntero válido
//POSCONDICIÓN: Devuelve 0 si el guardado de datos fue exitoso, 1 en caso de error.
void guardar_jugadores(jug_vect *);

#endif