#include "conexiones.h"
#include "salas.h"

int cargar_conex(vector_conex *conex){
    FILE *f_conex;
    char *token;
    //creamos la cadena para leer linea por linea
    char cadena[512];
    conex->conexreg=0;
    
    // inicializamos a NULL en lugar de hacer malloc. IMPORTANTE (por lo que sea)
    // Así el primer realloc se encarga de crear el espacio correctamente.
    conex->conexvec = NULL;
    
    f_conex=fopen("Conexiones.txt","r");
    //error al abrir el archivo.
    if (f_conex==NULL){
        return 0;
    }
    
    while(fgets(cadena,sizeof(cadena),f_conex)!=NULL){
        conex->conexvec=(conexion_str *)realloc(conex->conexvec,(conex->conexreg+1)*sizeof(conexion_str));
        
        // comprobar seguridad por si realloc falla.
        if (conex->conexvec == NULL) {
            fclose(f_conex);
            return 0;
        }

        //Para la conexion que son caracteres.
        token=strtok(cadena,"-");
        if (token!=NULL){
            // NUEVO: Dejamos espacio para el terminador nulo (-1)
            strncpy(conex->conexvec[conex->conexreg].id_conexion,token,MAXCON - 1);
            //aseguramos terminador nulo
            conex->conexvec[conex->conexreg].id_conexion[MAXCON - 1] = '\0';
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
        // hay que usar NULL, si uso "cadena" la lectura se reiniciará.
        token=strtok(NULL,"-");
        if (token!=NULL){
            // ajustado a MAXEST y asignado al atributo correcto ('estado', no 'id_conexion')
            strncpy(conex->conexvec[conex->conexreg].estado,token,MAXEST - 1);
            //aseguramos terminador nulo
            conex->conexvec[conex->conexreg].estado[MAXEST - 1] = '\0';
        }
        token=strtok(NULL,"\n");
        if (token != NULL){
            // Copiamos el texto de la condición (ej: "0", "P01", "OB03")
            strncpy(conex->conexvec[conex->conexreg].condicion, token, MAXC - 1);
            // Aseguramos el terminador nulo
            conex->conexvec[conex->conexreg].condicion[MAXC- 1] = '\0';
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

void escribir_conexiones(vector_conex conex, int num_salas) {
    printf("Conexiones:\n");
    // Validar que los índices de origen y destino estén dentro del rango de salas
    if (conex.conexvec[num_salas].id_origen >= 0 && conex.conexvec[num_salas].id_origen < num_salas &&
        conex.conexvec[num_salas].id_destino >= 0 && conex.conexvec[num_salas].id_destino < num_salas) {
        printf("ID: %s | Origen: %d | Destino: %d | Estado: %s\n",
                conex.conexvec[num_salas].id_conexion,
                conex.conexvec[num_salas].id_origen,
                conex.conexvec[num_salas].id_destino,
                conex.conexvec[num_salas].estado);
    } else {
        printf("Conexión ID: %s tiene índices de origen o destino fuera de rango.\n",
                conex.conexvec[num_salas].id_conexion);
    }
}