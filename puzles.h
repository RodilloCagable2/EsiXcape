#ifndef PUZLE_H
#define PUZLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "salas.h"

#define max_id_puzle 4
#define max_nombre_puzle 16
#define max_tipo_puzle 10
#define max_descripcion_puzle 151
#define max_solucion_puzle 51

typedef struct{
    char id_puzle[max_id_puzle];
    char nombre[max_nombre_puzle];
    int id_sala;
    char tipo[max_tipo_puzle];
    char descripcion[max_descripcion_puzle];
    char solucion[max_solucion_puzle];
}puzle;

//precondición: Entra un array de puzles vacío.
//postcondición: carga todos los datos en las estructuras/registros, desde el archivo de texto "puzles.txt".
void cargar_puzles(puzle **);

//precondición: Entra un array de puzles y un id de un puzle existente.
//postcondición: Imprime por pantalla el puzle a averiguar.
void mostrar_puzle(puzle *, int);

//precondición: Entra el puzle.
//postcondición: Imprime por pantalla la solución del puzle dado.
void mostrar_solucion_puzle(puzle *, int);

//precondición: Entra un array de puzles y un id de puzle.
//postcondición: Devuelve el índice del puzle en el array o -1 si no se encuentra.
int buscar_puzle(puzle *, char *);

//Precondición: Entra un array de puzles, un id de puzle y una respuesta dada por el jugador.
//Postcondición: Devuelve 1 si la respuesta es correcta, 0 si es incorrecta y -1 si el puzle no existe o la respuesta es inválida.
int comprobar_solucion_puzle(puzle *, char *, char *);

//precondición: Entra una cadena de caracteres.
//postcondición: Elimina los caracteres \r y \n, reemplazándolos por el carácter nulo, para que funcione el strcmp.   
void limpiar_cadena(char *);


#endif