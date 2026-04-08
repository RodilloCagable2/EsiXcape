#include "puzles.h"

void carga_puzles(puzle **p){

    FILE *f = fopen("puzles.txt", "r");

    if(f == NULL){
        printf("Error al abrir el archivo de puzles.\n");
        return;
    }

    char linea[256];
    *p = NULL;
    int i = 0;

    while(fgets(linea, sizeof(linea), f) != NULL){

        puzle *temp = realloc(*p, (i + 1) * sizeof(puzle));
        
        if(temp == NULL){
            printf("Error al asignar memoria para los puzles.\n");
            free(*p);
   
            fclose(f);
            return -1;
        }

        *p = temp;

        char *token = strtok(linea, "-");

        if(token != NULL){
            strncpy((*p)[i].id_puzle, token, max_id_puzle - 1);
            (*p)[i].id_puzle[max_id_puzle - 1] = '\0';
        }

        token = strtok(NULL, "-");
        if(token != NULL){
            strncpy((*p)[i].nombre, token, max_nombre_puzle - 1);
            (*p)[i].nombre[max_nombre_puzle - 1] = '\0';
        }

        token = strtok(NULL, "-");
        if(token != NULL){
            (*p)[i].id_sala = atoi(token);
        }

        token = strtok(NULL, "-");
        if(token != NULL){
            strncpy((*p)[i].tipo, token, max_tipo_puzle - 1);
            (*p)[i].tipo[max_tipo_puzle - 1] = '\0';
        }

        token = strtok(NULL, "-");
        if(token != NULL){
            strncpy((*p)[i].descripcion, token, max_descripcion_puzle - 1);
            (*p)[i].descripcion[max_descripcion_puzle - 1] = '\0';
        }

        token = strtok(NULL, "-\n");
        if(token != NULL){
            strncpy((*p)[i].solucion, token, max_solucion_puzle - 1);
            (*p)[i].solucion[max_solucion_puzle - 1] = '\0';
        }

        (i)++;
    }

    fclose(f);
}

void mostrar_puzle(puzle *p, int id_sala){
    printf("Id_puzle: %s\n", p[id_sala].id_puzle);
    printf("Puzle: %s\n", p[id_sala].nombre);
    printf("Id_sala: %d\n", p[id_sala].id_sala);
    printf("Tipo: %s\n", p[id_sala].tipo);
    printf("Descripción: %s\n", p[id_sala].descripcion);
    printf("Solución: %s\n", p[id_sala].solucion);
}

void mostrar_solucion_puzle(puzle *p, int id_puzle){
    printf("La solución del puzle %s es: %s\n", p[id_puzle - 1].nombre, p[id_puzle - 1].solucion);
}

int buscar_puzle(puzle *p, char* id_puzle){
    for(int i = 0; p[i].id_puzle[0] != '\0'; i++){
        if(strcmp(p[i].id_puzle, id_puzle) == 0){
            return i + 1;
        }
    }
    return -1;
}

int comprobar_solucion_puzle(puzle *p, char* id_puzle, char* respuesta){
    int indice = buscar_puzle(p, id_puzle);
    if(indice == -1){
        printf("Puzle no encontrado.\n");
        return -1;
    }

    if(respuesta == NULL || strlen(respuesta) >= max_solucion_puzle){
        printf("Respuesta inválida.\n");
        return -1;
    }
    limpiar_cadena(p[indice - 1].solucion);
    if(strcmp(p[indice - 1].solucion, respuesta) == 0){
        printf("Respuesta correcta.\n");
        return 1;
    } else {
        printf("Respuesta incorrecta.\n");
        return 0;
    }
}

void limpiar_cadena(char *s) {
    int i = 0;

    while (s[i] != '\0') {
        if (s[i] == '\n' || s[i] == '\r') {
            s[i] = '\0';
        }
        i++;
    }
}