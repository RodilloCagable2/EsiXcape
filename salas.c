//atoi= ascii to character
//strtok: la primera vez le das el nombre de la cadena, la segunda vez le pones null, strtok(NULL,"-")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNOMB 30
#define MAXDESC 150
#define MAXTYPE 8

//estructura de la sala.
typedef struct{
    int id_sala;
    char nombr_sala[MAXNOMB];
    char type_sala[MAXTYPE];
    char desrc_sala[MAXDESC];
}sala_str;

typedef struct {
    sala_str *vctr_sala;
    int numreg;
}vector_sala;
//PODEMOS PONER UN TERMINADOR NULO SI HACE FALTA, PERO SI NO DA IGUAL
int cargar_salas (vector_sala *sala){
    FILE *f_sala;
    char *token;
    char cadena[512];
    sala->numreg=0;
    f_sala = fopen("salas.txt", "r");
    if (f_sala == NULL) {
        return 0; // Error al abrir el fichero
    }
    //inicializamos el malloc en esta linea.
    sala->vctr_sala=(sala_str *) malloc(1*sizeof(sala_str));
    while(fgets(sala,500000, f_sala)!=NULL){
        //vamos leyendo poco a poco el archivo en el orden especificado.
        sala->vctr_sala = (sala_str *) realloc(sala->vctr_sala, (sala->numreg+ 1) * sizeof(sala_str));
        token=strtok(cadena,"-");
        if (token != NULL){
            sala->vctr_sala[sala->numreg].id_sala=atoi(token);
        }
        //Para cargar el nombre de la sala.
        token=strtok(NULL, "-");
        if (token!=NULL){
            strncpy(sala->vctr_sala[sala->numreg].nombr_sala,token,MAXNOMB);
        }
        
        //SI HIRIERA FALTA MAXNOMB -1
        //sala->vctr_sala[sala->numreg].nombr_sala='\0'

        //haz lo que necesites con el token
        //el token sirve para ir ubicandonos en el .txt
        token=strtok(NULL,"-");
        if (token != NULL){
            strncpy(sala->vctr_sala[sala->numreg].type_sala,token,MAXTYPE);
        }

        token=strtok(NULL,"\n");
        if (token != NULL){
            strncpy(sala->vctr_sala[sala->numreg].desrc_sala,token,MAXDESC);
        }
        sala->numreg++;
    }
    
    fclose (f_sala);
    return 1;
}