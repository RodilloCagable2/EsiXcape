#include "partida.h"

void limpiar_cadena_partida(char *s){
    int i = 0;

    while(s[i] != '\0'){
        if(s[i] == '\n' || s[i] == '\r'){
            s[i] = '\0';
            break;
        }
        i++;
    }
}

int existe_jugador_partida(partida *p, int num_partidas, char *id_jugador){
    int i;

    for(i = 0; i < num_partidas; i++){
        if(strcmp(p[i].id_jugador, id_jugador) == 0){
            return 1;
        }
    }

    return 0;
}

int cargar_partida(partida **p){
    FILE *f;
    char linea[256];
    int num_partidas = 0;
    int actual = -1;
    int ignorar_bloque = 0;

    f = fopen("partida.txt", "r");
    if(f == NULL){
        printf("Error al abrir el archivo partida.txt\n");
        *p = NULL;
        return 0;
    }

    *p = NULL;

    while(fgets(linea, sizeof(linea), f) != NULL){
        limpiar_cadena_partida(linea);

        if(strlen(linea) == 0){
            continue;
        }

        if(strncmp(linea, "JUGADOR:", 8) == 0){
            char id_jugador[max_id_jugador];

            sscanf(linea, "JUGADOR: %2s", id_jugador);

            /* Cada JUGADOR inicia una partida nueva */
            /* Si ese jugador ya tenía partida, se ignora ese bloque */
            if(existe_jugador_partida(*p, num_partidas, id_jugador)){
                printf("Aviso: el jugador %s ya tiene una partida guardada. Bloque ignorado.\n", id_jugador);
                actual = -1;
                ignorar_bloque = 1;
            }else{
                partida *temp = realloc(*p, (num_partidas + 1) * sizeof(partida));
                if(temp == NULL){
                    printf("Error al reservar memoria para partidas.\n");
                    fclose(f);
                    return num_partidas;
                }

                *p = temp;
                actual = num_partidas;
                ignorar_bloque = 0;

                /* Inicializar la nueva partida */
                (*p)[actual].id_jugador[0] = '\0';
                (*p)[actual].id_sala_actual[0] = '\0';
                (*p)[actual].objetos = NULL;
                (*p)[actual].conexiones = NULL;
                (*p)[actual].puzles = NULL;

                strncpy((*p)[actual].id_jugador, id_jugador, max_id_jugador - 1);
                (*p)[actual].id_jugador[max_id_jugador - 1] = '\0';

                num_partidas++;
            }
        }
        else if(!ignorar_bloque && actual != -1 && strncmp(linea, "SALA:", 5) == 0){
            sscanf(linea, "SALA: %2s", (*p)[actual].id_sala_actual);
        }
        else if(!ignorar_bloque && actual != -1 && strncmp(linea, "OBJETO:", 7) == 0){
            char id_obj[max_id_objeto];
            char localizacion[max_localizacion];
            int n = 0;
            objeto_partida *temp_obj;

            if(sscanf(linea, "OBJETO: %4[^-]-%10s", id_obj, localizacion) == 2){
                if((*p)[actual].objetos != NULL){
                    while((*p)[actual].objetos[n].id_objeto[0] != '\0'){
                        n++;
                    }
                }

                temp_obj = realloc((*p)[actual].objetos, (n + 2) * sizeof(objeto_partida));
                if(temp_obj == NULL){
                    printf("Error al reservar memoria para objetos en la partida.\n");
                    fclose(f);
                    return num_partidas;
                }

                (*p)[actual].objetos = temp_obj;

                strncpy((*p)[actual].objetos[n].id_objeto, id_obj, max_id_objeto - 1);
                (*p)[actual].objetos[n].id_objeto[max_id_objeto - 1] = '\0';

                strncpy((*p)[actual].objetos[n].localizacion, localizacion, max_localizacion - 1);
                (*p)[actual].objetos[n].localizacion[max_localizacion - 1] = '\0';

                /* Marca de fin */
                (*p)[actual].objetos[n + 1].id_objeto[0] = '\0';
                (*p)[actual].objetos[n + 1].localizacion[0] = '\0';
            }
        }
        else if(!ignorar_bloque && actual != -1 && strncmp(linea, "CONEXION:", 9) == 0){
            char id_con[max_id_conexion];
            char estado[max_estado_conexion];
            int n = 0;
            conexion_partida *temp_con;

            if(sscanf(linea, "CONEXION: %3[^-]-%10s", id_con, estado) == 2){
                if((*p)[actual].conexiones != NULL){
                    while((*p)[actual].conexiones[n].id_conexion[0] != '\0'){
                        n++;
                    }
                }

                temp_con = realloc((*p)[actual].conexiones, (n + 2) * sizeof(conexion_partida));
                if(temp_con == NULL){
                    printf("Error al reservar memoria para conexiones en la partida.\n");
                    fclose(f);
                    return num_partidas;
                }

                (*p)[actual].conexiones = temp_con;

                strncpy((*p)[actual].conexiones[n].id_conexion, id_con, max_id_conexion - 1);
                (*p)[actual].conexiones[n].id_conexion[max_id_conexion - 1] = '\0';

                strncpy((*p)[actual].conexiones[n].estado, estado, max_estado_conexion - 1);
                (*p)[actual].conexiones[n].estado[max_estado_conexion - 1] = '\0';

                /* Marca de fin */
                (*p)[actual].conexiones[n + 1].id_conexion[0] = '\0';
                (*p)[actual].conexiones[n + 1].estado[0] = '\0';
            }
        }
        else if(!ignorar_bloque && actual != -1 && strncmp(linea, "PUZLE:", 6) == 0){
            char id_puz[max_id_puzle];
            char estado[max_estado_puzle];
            puzle_partida *nuevo;
            puzle_partida *aux;

            if(sscanf(linea, "PUZLE: %3[^-]-%10s", id_puz, estado) == 2){
                nuevo = (puzle_partida *)malloc(sizeof(puzle_partida));
                if(nuevo == NULL){
                    printf("Error al reservar memoria para puzles en la partida.\n");
                    fclose(f);
                    return num_partidas;
                }

                strncpy(nuevo->id_puzle, id_puz, max_id_puzle - 1);
                nuevo->id_puzle[max_id_puzle - 1] = '\0';

                strncpy(nuevo->estado, estado, max_estado_puzle - 1);
                nuevo->estado[max_estado_puzle - 1] = '\0';

                nuevo->siguiente = NULL;

                if((*p)[actual].puzles == NULL){
                    (*p)[actual].puzles = nuevo;
                }else{
                    aux = (*p)[actual].puzles;
                    while(aux->siguiente != NULL){
                        aux = aux->siguiente;
                    }
                    aux->siguiente = nuevo;
                }
            }
        }
    }

    fclose(f);
    return num_partidas;
}

void mostrar_partida(partida *p) {
    printf("Jugador: %s\n", p->id_jugador);
    printf("Sala actual: %s\n", p->id_sala_actual);

    printf("Objetos:\n");
    if(p->objetos != NULL){
        int i = 0;
        while(p->objetos[i].id_objeto[0] != '\0'){
            printf("  - ID: %s, Localización: %s\n", p->objetos[i].id_objeto, p->objetos[i].localizacion);
            i++;
        }
    }

    printf("Conexiones:\n");
    if(p->conexiones != NULL){
        int i = 0;
        while(p->conexiones[i].id_conexion[0] != '\0'){
            printf("  - ID: %s, Estado: %s\n", p->conexiones[i].id_conexion, p->conexiones[i].estado);
            i++;
        }
    }

    printf("Puzles:\n");
    puzle_partida *aux = p->puzles;
    while(aux != NULL){
        printf("  - ID: %s, Estado: %s\n", aux->id_puzle, aux->estado);
        aux = aux->siguiente;
    }

    printf("\n");
}

void escribir_una_partida(FILE *f, partida *p){
    int i;

    if(f == NULL || p == NULL){
        return;
    }

    fprintf(f, "JUGADOR: %s\n", p->id_jugador);
    fprintf(f, "SALA: %s\n", p->id_sala_actual);

    if(p->objetos != NULL){
        i = 0;
        while(p->objetos[i].id_objeto[0] != '\0'){
            fprintf(f, "OBJETO: %s-%s\n",
                    p->objetos[i].id_objeto,
                    p->objetos[i].localizacion);
            i++;
        }
    }

    if(p->conexiones != NULL){
        i = 0;
        while(p->conexiones[i].id_conexion[0] != '\0'){
            fprintf(f, "CONEXION: %s-%s\n",
                    p->conexiones[i].id_conexion,
                    p->conexiones[i].estado);
            i++;
        }
    }

    if(p->puzles != NULL){
        i = 0;
        while(p->puzles[i].id_puzle[0] != '\0'){
            fprintf(f, "PUZLE: %s-%s\n",
                    p->puzles[i].id_puzle,
                    p->puzles[i].estado);
            i++;
        }
    }
}

void guardar_partida(partida *p){
    partida *partidas = NULL;
    FILE *f;
    int num_partidas, i;
    int encontrada = 0;

    if(p == NULL){
        printf("Error: la partida es NULL.\n");
        return;
    }

    /* Cargar todas las partidas existentes */
    num_partidas = cargar_partida(&partidas);

    /* Reescribir el fichero completo */
    f = fopen("partida.txt", "w");
    if(f == NULL){
        printf("Error al abrir el archivo partida.txt para guardar.\n");

        if(partidas != NULL){
            for(i = 0; i < num_partidas; i++){
                eliminar_partida(&partidas[i]);
            }
            free(partidas);
        }
        return;
    }

    /* Escribir todas las partidas, sustituyendo la del mismo jugador */
    for(i = 0; i < num_partidas; i++){
        if(strcmp(partidas[i].id_jugador, p->id_jugador) == 0){
            escribir_una_partida(f, p);
            encontrada = 1;
        }else{
            escribir_una_partida(f, &partidas[i]);
        }
    }

    /* Si no existía, añadirla al final */
    if(!encontrada){
        escribir_una_partida(f, p);
    }

    fclose(f);

    /* Liberar memoria de las partidas cargadas */
    if(partidas != NULL){
        for(i = 0; i < num_partidas; i++){
            eliminar_partida(&partidas[i]);
        }
        free(partidas);
    }
}

void eliminar_partida(partida *p){
    partida *partidas = NULL;
    FILE *f;
    int num_partidas, i;

    if(p == NULL){
        return;
    }

    /* 1. Cargar todas las partidas del fichero */
    num_partidas = cargar_partida(&partidas);

    /* 2. Reescribir el fichero sin la partida del jugador actual */
    f = fopen("partida.txt", "w");
    if(f == NULL){
        printf("Error al abrir partida.txt para eliminar la partida.\n");
    }else{
        for(i = 0; i < num_partidas; i++){
            if(strcmp(partidas[i].id_jugador, p->id_jugador) != 0){
                escribir_una_partida(f, &partidas[i]);
            }
        }
        fclose(f);
    }

    /* 3. Liberar las partidas temporales cargadas del fichero */
    if(partidas != NULL){
        for(i = 0; i < num_partidas; i++){
            free(partidas[i].objetos);
            free(partidas[i].conexiones);
            free(partidas[i].puzles);

            partidas[i].objetos = NULL;
            partidas[i].conexiones = NULL;
            partidas[i].puzles = NULL;
        }
        free(partidas);
    }

    /* 4. Liberar la memoria de la partida recibida */
    free(p->objetos);
    free(p->conexiones);
    free(p->puzles);

    p->objetos = NULL;
    p->conexiones = NULL;
    p->puzles = NULL;

    p->id_jugador[0] = '\0';
    p->id_sala_actual[0] = '\0';
}