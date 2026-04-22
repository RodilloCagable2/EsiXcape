#include "puzles.h"

void cargar_puzles(puzle **p){
    FILE *f = fopen("puzles.txt", "r");
    char linea[256];
    int i = 0;

    if (p == NULL) {
        return;
    }

    *p = NULL;
    if(f == NULL){
        printf("Error al abrir el archivo de puzles.\n");
        return;
    }

    while(fgets(linea, sizeof(linea), f) != NULL){
        puzle *temp = (puzle *)realloc(*p, (size_t)(i + 2) * sizeof(puzle));
        char *token;

        if(temp == NULL){
            printf("Error al asignar memoria para los puzles.\n");
            free(*p);
            *p = NULL;
            fclose(f);
            return;
        }

        *p = temp;
        memset(&(*p)[i], 0, sizeof(puzle));
        memset(&(*p)[i + 1], 0, sizeof(puzle));

        token = strtok(linea, "-");
        if(token != NULL){
            strncpy((*p)[i].id_puzle, token, max_id_puzle - 1);
        }

        token = strtok(NULL, "-");
        if(token != NULL){
            strncpy((*p)[i].nombre, token, max_nombre_puzle - 1);
        }

        token = strtok(NULL, "-");
        if(token != NULL){
            (*p)[i].id_sala = atoi(token);
        }

        token = strtok(NULL, "-");
        if(token != NULL){
            strncpy((*p)[i].tipo, token, max_tipo_puzle - 1);
        }

        token = strtok(NULL, "-");
        if(token != NULL){
            strncpy((*p)[i].descripcion, token, max_descripcion_puzle - 1);
        }

        token = strtok(NULL, "\n");
        if(token != NULL){
            strncpy((*p)[i].solucion, token, max_solucion_puzle - 1);
        }

        limpiar_cadena((*p)[i].solucion);
        i++;
    }

    fclose(f);
}

void mostrar_puzle(puzle *p, int id_sala){
    int i;
    if (p == NULL) return;
    for (i = 0; p[i].id_puzle[0] != '\0'; i++) {
        if (p[i].id_sala == id_sala) {
            printf("Puzle: %s\n", p[i].descripcion);
            return;
        }
    }
}

void mostrar_solucion_puzle(puzle *p, int id_puzle){
    if (p == NULL || id_puzle < 0) return;
    printf("La solucion del puzle %s es: %s\n", p[id_puzle].nombre, p[id_puzle].solucion);
}

int buscar_puzle(puzle *p, char* id_puzle){
    int i;
    if (p == NULL || id_puzle == NULL) return -1;
    for(i = 0; p[i].id_puzle[0] != '\0'; i++){
        if(strcmp(p[i].id_puzle, id_puzle) == 0){
            return i;
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

    limpiar_cadena(respuesta);
    if(strcmp(p[indice].solucion, respuesta) == 0){
        return 1;
    }
    return 0;
}

void limpiar_cadena(char *s) {
    int i = 0;
    if (s == NULL) return;
    while (s[i] != '\0') {
        if (s[i] == '\n' || s[i] == '\r') {
            s[i] = '\0';
            break;
        }
        i++;
    }
}
