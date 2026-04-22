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

int cargar_salas(vector_sala *sala);
int describir_salas(vector_sala sala, int id_sala);
int entrar_otra_sala(vector_conex *conex, vector_sala *sala, int *salactual);

#endif