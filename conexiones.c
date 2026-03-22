#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXEST 20
#define MAXCON 3
typedef struct{
    char id_conexion[MAXCON];
    int id_origen;
    int id_destino;
    char estado [MAXEST];
    int condicion;
}conexion_str;
typedef struct{
    //vector a conexiones para poder hacerle malloc y realloc las veces que haga falta.
    conexion_str *conexvec;
    int conexreg;
}vector_conex;
int cargar_conex(vector_conex *conex){
    FILE *f_conex;
    char *token;
    //creamos la cadena para 
    char cadena[512];
    f_conex=fopen("conexiones.txt","w");
    //error al abrir el archivo.
    if (f_conex==NULL){
        return 0;
    }
    //hacemos malloc para poder ir metiendo los datos que haya que meter.
    conex->conexvec=(conexion_str*)malloc(1*sizeof(conexion_str));
    while(fgets(conex,50000,f_conex)!=NULL){
        conex->conexvec=(conexion_str *)realloc(conex->conexvec,(conex->conexreg+1)*sizeof(conexion_str));
        //Para la conexion que son caracteres.
        token=strtok(cadena,"-");
        if (token!=NULL){
            strncpy(conex->conexvec[conex->conexreg].id_conexion,token,MAXCON);
        }

        token=strtok(NULL,"-");
        if (token!=NULL){
            conex->conexvec[conex->conexreg].id_origen=atoi(token);
        }

        token=strtok(NULL,"-");
        if (token!=NULL){
            conex->conexvec[conex->conexreg].id_destino=atoi(token);
        }
        //el ultimo dato será un salto de linea
        token=strtok(cadena,"\n");
        if (token!=NULL){
            strncpy(conex->conexvec[conex->conexreg].estado,token,MAXEST);
        }
        conex->conexreg++;
    }
    fclose(f_conex);
    //si ha salido todo bien devuelve 1.
    return 1;
}
//planteamiento si tenemos que guardar las cosas por separado.
/*int guardar_salas(vector_conex *conex,jug_vect *jug){
    FILE *f_partida;
    f_partida=fopen("Partida.txt","w");
    if (f_partida==NULL){
        return 0; //error de carga
    }

}*/
