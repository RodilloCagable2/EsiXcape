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
    
    f_conex=fopen("conexiones.txt","r");
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
