//atoi= ascii to character
//strtok: la primera vez le das el nombre de la cadena, la segunda vez le pones null, strtok(NULL,"-")

#include "salas.h"
#include "conexiones.h"
//estructura de la sala.

//PODEMOS PONER UN TERMINADOR NULO SI HACE FALTA, PERO SI NO DA IGUAL
int cargar_salas (vector_sala *sala){
    FILE *f_sala;
    char *token;
    char cadena[512];
    sala->numreg=0;
    sala->vctr_sala = NULL;
    f_sala = fopen("Salas.txt", "r");
    if (f_sala == NULL) {
        return 0; // Error al abrir el fichero
    }
    //sala vctr sala inicializar a nulo así puedes hacer realloc 
    while(fgets(cadena,sizeof(cadena), f_sala)!=NULL){
        //vamos leyendo poco a poco el archivo en el orden especificado.
        sala->vctr_sala = (sala_str *) realloc(sala->vctr_sala, (sala->numreg+ 1) * sizeof(sala_str));
        if (sala ->vctr_sala==NULL){
            fclose(f_sala);
            return 0;
        }
        token=strtok(cadena,"-");
        if (token != NULL){
            sala->vctr_sala[sala->numreg].id_sala=atoi(token);
        }
        //Para cargar el nombre de la sala.
        token=strtok(NULL, "-");
        if (token!=NULL){
            strncpy(sala->vctr_sala[sala->numreg].nombr_sala,token,MAXNOMB-1);
            sala->vctr_sala[sala->numreg].nombr_sala[MAXNOMB-1]='\0';
        }
        
        //SI HIRIERA FALTA MAXNOMB -1(hizo falta)
        //sala->vctr_sala[sala->numreg].nombr_sala='\0'

        //haz lo que necesites con el token
        //el token sirve para ir ubicandonos en el .txt
        token=strtok(NULL,"-");
        if (token != NULL){
            strncpy(sala->vctr_sala[sala->numreg].type_sala,token,MAXTYPE-1);
            sala->vctr_sala[sala->numreg].type_sala[MAXTYPE-1]='\0';
        }

        token=strtok(NULL,"\n");
        if (token != NULL){
            strncpy(sala->vctr_sala[sala->numreg].desrc_sala,token,MAXDESC-1);
            sala->vctr_sala[sala->numreg].desrc_sala[MAXDESC-1]='\0';
        }
        sala->numreg++;
    }
    
    fclose (f_sala);
    return 1;
}
//int x es para dirigirme a la sala la cual podemos estar.

int describir_salas(vector_sala sala, int id_sala){
    int i;

    for (i = 0; i < sala.numreg; i++){
        if (sala.vctr_sala[i].id_sala == id_sala){
            printf("\n=== %s ===\n", sala.vctr_sala[i].nombr_sala);
            printf("Tipo: %s\n", sala.vctr_sala[i].type_sala);
            printf("%s\n", sala.vctr_sala[i].desrc_sala);
            return 1;
        }
    }

    printf("\nNo se ha encontrado la sala con id %d\n", id_sala);
    return 0;
}
//id actual de la sala
int entrar_otra_sala(vector_conex *conex, vector_sala *sala, int *salactual){
    int i,j=0;
    int x;
    int existe=0;
    char *nombre_dest;
    printf("\nA partir de aqui podemos ir a las siguientes salas:\n");
    for (i=0;i<conex->conexreg;i++){
        if(*salactual == conex->conexvec[i].id_origen){
            // busco el nombre de la sala destino en el vector de salas
            nombre_dest = "Desconocida";
            for(j = 0; j < sala->numreg; j++) {
                if(sala->vctr_sala[j].id_sala == conex->conexvec[i].id_destino) {
                    nombre_dest = sala->vctr_sala[j].nombr_sala;
                    break;
                }
            }
            printf(" -> Sala %d (%s) [%s]\n", 
                conex->conexvec[i].id_destino,nombre_dest,conex->conexvec[i].estado);
        }
    }
    do{
        printf("\nescribe el numero de las sala a la que quiere viajar correspondientes: ");
        if (scanf("%d", &x) != 1) { 
            while(getchar() != '\n'); 
            printf("\nEntrada no valida. Escribe un numero.\n");
            continue;
        }
        for (i = 0; i < conex->conexreg; i++) {
            if (*salactual == conex->conexvec[i].id_origen && x == conex->conexvec[i].id_destino) {
                existe = 1;
                
                // Comprobar si está "Activa". strcmp devuelve 0 si son iguales.
                if (strcmp(conex->conexvec[i].estado, "Activa") == 0) {
                    *salactual = x;
                    printf("Viajando a la sala %d...\n", *salactual);
                } else {
                    // Si está bloqueada,le avisamos y no cambiamos el *salactual
                    printf("\nLa conexion está %s. Tienes que resolver un acertijo o usar un objeto.\n", conex->conexvec[i].estado);
                }
                break; // Salimos del for de comprobación porque ya encontramos la conexión
            }
        }
        if (existe == 0) {
            printf("\nOpcion incorrecta. No hay conexión a esa sala desde aqui.\n");
        }
    }while(existe==0);
    return 1;
}
