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
    f_sala = fopen("salas.txt", "r");
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
    int i, j;
    int x;
    int existe = 0;
    char *nombre_dest;
    int sala_objetivo; // NUEVO: Variable para saber a qué extremo de la conexión vamos

    printf("\nA partir de aqui podemos ir a las siguientes salas:\n");
    
    // Bucle para mostrar las opciones disponibles
    for (i = 0; i < conex->conexreg; i++){
        // ¿Estamos en el origen o estamos en el destino de esta conexión?
        if(*salactual == conex->conexvec[i].id_origen || *salactual == conex->conexvec[i].id_destino){
            
            if (*salactual == conex->conexvec[i].id_origen) {
                sala_objetivo = conex->conexvec[i].id_destino; // Vamos hacia adelante
            } else {
                sala_objetivo = conex->conexvec[i].id_origen;  // Vamos hacia atrás
            }

            // Buscamos el nombre de la sala_objetivo
            nombre_dest = "Desconocida"; 
            for(j = 0; j < sala->numreg; j++) {
                if(sala->vctr_sala[j].id_sala == sala_objetivo) {
                    nombre_dest = sala->vctr_sala[j].nombr_sala;
                    break;
                }
            }

            printf(" -> Sala %d (%s) [%s]\n", 
                sala_objetivo, 
                nombre_dest,
                conex->conexvec[i].estado);
        }
    }

    do{
        printf("\nEscribe el numero de la sala a la que quieres viajar: ");
        
        if (scanf("%d", &x) != 1) { 
            while(getchar() != '\n'); 
            printf("Entrada no valida. Escribe un numero.\n");
            continue;
        }

        for (i = 0; i < conex->conexreg; i++) {
            // Caso A: Viaje de ida (Actual == Origen y X == Destino)
            // Caso B: Viaje de vuelta (Actual == Destino y X == Origen)
            if ((*salactual == conex->conexvec[i].id_origen && x == conex->conexvec[i].id_destino) ||
                (*salactual == conex->conexvec[i].id_destino && x == conex->conexvec[i].id_origen)) {
                
                existe = 1; 
                
                if (strcmp(conex->conexvec[i].estado, "Activa") == 0) {
                    *salactual = x;
                    printf("Viajando a la sala %d...\n", *salactual);
                } else {
                    printf("\nLa conexion esta %s. Tienes que resolver un acertijo o usar un objeto.\n", 
                           conex->conexvec[i].estado);
                }
                break; 
            }
        }

        if (existe == 0) {
            printf("\nOpcion incorrecta. No hay conexion a esa sala desde aqui.\n");
        }
    } while(existe == 0);
    
    return 1;
}
