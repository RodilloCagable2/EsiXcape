#ifndef SALAS_H
#define SALAS_H

#define MAXNOMB 30
#define MAXDESC 150
#define MAXTYPE 8
#define NUMSAL 11

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conexiones.h"

typedef struct{
    int id_sala;
    char nombr_sala[MAXNOMB];
    char type_sala[MAXTYPE];
    char desrc_sala[MAXDESC];
} sala_str;

typedef struct {
    sala_str *vctr_sala;
    int numreg;
} vector_sala;
//cabecera: int cargar_salas(vector_sala *);
//precondición:vector sala ya inicializado
//postcondición:devuelve las salas guardadas del fichero.txt correspondiente
int cargar_salas(vector_sala *sala);
//cabecera:int describir_salas(vector_sala, int );
//precondicion:sala tiene que tener los datos una vez pasado por cargar_salas y id_sala tienen que tener el id de la sala
//postcondicion: devuelve la descripcion correspondiente de la sala
int describir_salas(vector_sala sala, int id_sala);
//cabecera:int entrar_otra_sala(vector_conex *, vector_sala *, int *);
//precondicion:vector conex ya declarado y pasada por las funciones de cargar correspondientes, al igual que vector sala y la sala actual también dada (con un entero, ya que si no tiene no estaríamos en una sala)
//postcondicion:el jugador puede entrar en otra sala.
int entrar_otra_sala(vector_conex *conex, vector_sala *sala, int *salactual);
#endif