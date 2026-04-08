//atoi= ascii to character
//strtok: la primera vez le das el nombre de la cadena, la segunda vez le pones null, strtok(NULL,"-")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "salas.h"
#include "conexiones.h"
//estructura de la sala.

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
    //sala vctr sala inicializar a nulo así puedes hacer realloc 
    sala->vctr_sala=(sala_str *) malloc(1*sizeof(sala_str));
    while(fgets(sala,sizeof(cadena), f_sala)!=NULL){
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
//int x es para dirigirme a la sala la cual podemos estar.

int describir_sala(vector_conex *conex,vector_sala *sala,int x){
    if (strcmp("SALIDA",sala->vctr_sala[x-1].type_sala)){
        printf("ENHORABUENA !");
        return 2;// Salida exitosa del programa y vuelta al menú principal.
    }
    printf("%s",sala->vctr_sala[x-1].desrc_sala);
    return 1;
}
//id actual de la sala
int entrar_otra_sala(vector_conex *conex, vector_sala *sala, int *salactual){
    int i;
    int j=0;
    int x;
    int existe=0;
    int k;
    int *guarda_sala;
    guarda_sala=(int *)malloc(1*sizeof(int));
    pritnf("a partir de aqui podemos ir a las siguientes salas:\n");
    for (i=0;i<conex->conexreg;i++){
        if(*salactual==conex->conexvec[i].id_origen){
            guarda_sala=(int *)realloc(guarda_sala,i*sizeof(int));

            printf(" %s, numero %d",sala->vctr_sala[conex->conexvec[i].id_destino-1].nombr_sala,sala->vctr_sala[conex->conexvec[i].id_destino-1].id_sala);
            /*guarda_sala[j]=sala->vctr_sala[conex->conexvec[i].id_origen-1].id_sala;
            j++;*/
        }
    }
    do{
        printf("\nescribe el numero de las sala a la que quiere viajar correspondientes: ");
        scanf("%d",&x);
        for (i = 0; i < conex->conexreg; i++) {
            if (*salactual == conex->conexvec[i].id_origen && x == conex->conexvec[i].id_destino) {
                existe = 1; // La conexión existe
                
                // Comprobar si está "Activa". strcmp devuelve 0 si son iguales.
                if (strcmp(conex->conexvec[i].estado, "Activa") == 0) {
                    *salactual = x;
                    printf("Viajando a la sala %d...\n", *salactual);
                } else {
                    // Si está bloqueada, avisamos y no cambiamos el *salactual
                    printf("\nLa conexión está %s. Tienes que resolver un acertijo o usar un objeto.\n", conex->conexvec[i].estado);
                }
                break; // Salimos del for de comprobación porque ya encontramos la conexión
            }
        }
        if (existe == 0) {
            printf("\nOpción incorrecta. No hay conexión a esa sala desde aquí.\n");
        }
    }while(existe==0);
    return 1;
}
    /*for (k<0;k<=j;k++){
        if (scanf());
    }*/
    /*if (strcmp("Activa",conex->conexvec[*idactual-1].estado)){
        *idactual=conex->conexvec[*idactual].id_destino;
        return 1;
    }else{
        printf("necesitas resolver un puzzle para resolver esta sala.");
        return 1;
    }*/
      /*while(k<=j){
            if (x==guarda_sala[k] && strcmp("Activa",conex->conexvec[k].estado)){
                *salactual=x;
                existe=1;
            }else if (!strcmp("Activa",conex->conexvec[k].estado)){
                printf("\ntienes que resolver el acertijo u usar un objeto.");
            }
            k++;*/