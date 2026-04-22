#ifndef CONEXIONES_H
#define CONEXIONES_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAXEST 20
#define MAXCON 4
#define MAXC 5


typedef struct{
    char id_conexion[MAXCON];
    int id_origen;
    int id_destino;
    char estado [MAXEST];
    char condicion[MAXC];
}conexion_str;


typedef struct{
    //vector a conexiones para poder hacerle malloc y realloc las veces que haga falta.
    conexion_str *conexvec;
    int conexreg;
}vector_conex;


int cargar_conex(vector_conex *);
void escribir_conexiones(vector_conex, int);

#endif