#ifndef OBJETOS_H_
#define OBJETOS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//---> ESTRUCTURAS: <---

typedef struct {
	char Id_obj[5];			//Identificador del objeto
	char Nomb_obj[16];		//Nombre del objeto
	char Describ[51];		//Descripción del objeto
	char Localiz[11];		//Localización del objeto
}objetos;

typedef struct {
	objetos obj[5];
}obj_vect;

//---> FUNCIONES: <---

//PRECONDICIÓN: Ninguna
//POSCONDICIÓN: Devuelve 0 si la carga de datos fue exitosa, 1 en caso de error. La estructura apuntada por j se actualiza con los datos cargados. 
void cargar_objetos(obj_vect *);

//PRECONDICIÓN: j debe ser un puntero válido
//POSCONDICIÓN: Devuelve 0 si el guardado de datos fue exitoso, 1 en caso de error.
void guardar_objetos(obj_vect *);

#endif