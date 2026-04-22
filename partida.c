#include "partida.h"

static int contar_puzles_array(puzle *p) {
    int i = 0;

    if (p == NULL) {
        return 0;
    }

    while (p[i].id_puzle[0] != '\0') {
        i++;
    }

    return i;
}

static int buscar_sala_inicial(vector_sala *salas) {
    int i;

    for (i = 0; i < salas->numreg; i++) {
        if (strcmp(salas->vctr_sala[i].type_sala, "INICIAL") == 0) {
            return salas->vctr_sala[i].id_sala;
        }
    }

    return 1;
}

static int inicializar_partida_base(partida *destino, int id_jugador_real) {
    vector_sala salas = {NULL, 0};
    vector_conex conexiones = {NULL, 0};
    obj_vect objetos;
    puzle *puzles = NULL;
    int num_puzles;
    int i;

    if (destino == NULL) {
        return 0;
    }

    memset(destino, 0, sizeof(partida));

    if (!cargar_salas(&salas)) {
        return 0;
    }
    if (!cargar_conex(&conexiones)) {
        free(salas.vctr_sala);
        return 0;
    }
    cargar_objetos(&objetos);

    cargar_puzles(&puzles);
    if (puzles == NULL) {
        free(salas.vctr_sala);
        free(conexiones.conexvec);
        return 0;
    }

    destino->id_jugador = id_jugador_real;
    destino->id_sala_actual = buscar_sala_inicial(&salas);
    destino->id_sala_anterior = -1;

    destino->objetos = (objeto_partida *)calloc(6, sizeof(objeto_partida));
    destino->conexiones = (conexion_partida *)calloc((size_t)conexiones.conexreg + 1, sizeof(conexion_partida));
    num_puzles = contar_puzles_array(puzles);
    destino->puzles = (puzle_partida *)calloc((size_t)num_puzles + 1, sizeof(puzle_partida));

    if (destino->objetos == NULL || destino->conexiones == NULL || destino->puzles == NULL) {
        free(salas.vctr_sala);
        free(conexiones.conexvec);
        free(puzles);
        liberar_partida_individual(destino);
        return 0;
    }

    for (i = 0; i < 5; i++) {
        strcpy(destino->objetos[i].id_objeto, objetos.obj[i].Id_obj);
        strcpy(destino->objetos[i].localizacion, objetos.obj[i].Localiz);
    }

    for (i = 0; i < conexiones.conexreg; i++) {
        strcpy(destino->conexiones[i].id_conexion, conexiones.conexvec[i].id_conexion);
        strcpy(destino->conexiones[i].estado, conexiones.conexvec[i].estado);
    }

    for (i = 0; i < num_puzles; i++) {
        strcpy(destino->puzles[i].id_puzle, puzles[i].id_puzle);
        strcpy(destino->puzles[i].estado, "Pendiente");
    }

    free(salas.vctr_sala);
    free(conexiones.conexvec);
    free(puzles);
    return 1;
}

static void aplicar_linea_objeto(partida *p, char *valor) {
    char id[max_id_objeto];
    char localizacion[max_localizacion];
    int i;

    if (sscanf(valor, "%4[^-]-%10[^\r\n]", id, localizacion) != 2) {
        return;
    }

    for (i = 0; p->objetos[i].id_objeto[0] != '\0'; i++) {
        if (strcmp(p->objetos[i].id_objeto, id) == 0) {
            strcpy(p->objetos[i].localizacion, localizacion);
            return;
        }
    }
}

static void aplicar_linea_conexion(partida *p, char *valor) {
    char id[max_id_conexion];
    char estado[max_estado_conexion];
    int i;

    if (sscanf(valor, "%3[^-]-%10[^\r\n]", id, estado) != 2) {
        return;
    }

    for (i = 0; p->conexiones[i].id_conexion[0] != '\0'; i++) {
        if (strcmp(p->conexiones[i].id_conexion, id) == 0) {
            strcpy(p->conexiones[i].estado, estado);
            return;
        }
    }
}

static void aplicar_linea_puzle(partida *p, char *valor) {
    char id[max_id_puzle];
    char estado[max_estado_puzle];
    int i;

    if (sscanf(valor, "%3[^-]-%10[^\r\n]", id, estado) != 2) {
        return;
    }

    for (i = 0; p->puzles[i].id_puzle[0] != '\0'; i++) {
        if (strcmp(p->puzles[i].id_puzle, id) == 0) {
            strcpy(p->puzles[i].estado, estado);
            return;
        }
    }
}

void limpiar_cadena_partida(char *s){
    int i = 0;

    while(s != NULL && s[i] != '\0'){
        if(s[i] == '\n' || s[i] == '\r'){
            s[i] = '\0';
            break;
        }
        i++;
    }
}

int buscar_indice_partida(partida *p, int num_partidas, int id_jugador){
    int i;

    for(i = 0; i < num_partidas; i++){
        if(p[i].id_jugador == id_jugador){
            return i;
        }
    }

    return -1;
}

int existe_jugador_partida(partida *p, int num_partidas, int id_jugador){
    return buscar_indice_partida(p, num_partidas, id_jugador) != -1;
}

int cargar_partida(partida **p){
    FILE *f = fopen("Partida.txt", "r");
    char linea[200];
    int num_partidas = 0;
    partida actual;
    int hay_partida_actual = 0;

    if (p == NULL) {
        return 0;
    }

    *p = NULL;
    if(f == NULL){
        return 0;
    }

    memset(&actual, 0, sizeof(actual));

    while(fgets(linea, sizeof(linea), f) != NULL){
        char *valor;

        if(linea[0] == '\n' || linea[0] == '\r'){
            continue;
        }

        if(strncmp(linea, "JUGADOR:", 8) == 0){
            if(hay_partida_actual){
                partida *tmp = (partida *)realloc(*p, (size_t)(num_partidas + 1) * sizeof(partida));
                if(tmp == NULL){
                    fclose(f);
                    liberar_partida_individual(&actual);
                    liberar_partidas(*p, num_partidas);
                    *p = NULL;
                    return 0;
                }
                *p = tmp;
                (*p)[num_partidas] = actual;
                num_partidas++;
                memset(&actual, 0, sizeof(actual));
            }

            valor = linea + 8;
            while(*valor == ' ' || *valor == '\t'){
                valor++;
            }
            limpiar_cadena_partida(valor);

            if(!inicializar_partida_base(&actual, atoi(valor))){
                fclose(f);
                liberar_partidas(*p, num_partidas);
                *p = NULL;
                return 0;
            }
            actual.id_sala_anterior = -1;
            hay_partida_actual = 1;
        }else if(strncmp(linea, "SALA:", 5) == 0 && hay_partida_actual){
            valor = linea + 5;
            while(*valor == ' ' || *valor == '\t'){
                valor++;
            }
            limpiar_cadena_partida(valor);
            actual.id_sala_actual = atoi(valor);
            actual.id_sala_anterior = -1;
        }else if(strncmp(linea, "OBJETO:", 7) == 0 && hay_partida_actual){
            valor = linea + 7;
            while(*valor == ' ' || *valor == '\t'){
                valor++;
            }
            limpiar_cadena_partida(valor);
            aplicar_linea_objeto(&actual, valor);
        }else if((strncmp(linea, "CONEXION:", 9) == 0 || strncmp(linea, "CONEXIÓN:", 10) == 0) && hay_partida_actual){
            valor = (strncmp(linea, "CONEXION:", 9) == 0) ? linea + 9 : linea + 10;
            while(*valor == ' ' || *valor == '\t'){
                valor++;
            }
            limpiar_cadena_partida(valor);
            aplicar_linea_conexion(&actual, valor);
        }else if(strncmp(linea, "PUZLE:", 6) == 0 && hay_partida_actual){
            valor = linea + 6;
            while(*valor == ' ' || *valor == '\t'){
                valor++;
            }
            limpiar_cadena_partida(valor);
            aplicar_linea_puzle(&actual, valor);
        }
    }

    if(hay_partida_actual){
        partida *tmp = (partida *)realloc(*p, (size_t)(num_partidas + 1) * sizeof(partida));
        if(tmp == NULL){
            fclose(f);
            liberar_partida_individual(&actual);
            liberar_partidas(*p, num_partidas);
            *p = NULL;
            return 0;
        }
        *p = tmp;
        (*p)[num_partidas] = actual;
        num_partidas++;
    }

    fclose(f);
    return num_partidas;
}

void escribir_una_partida(FILE *f, partida *p){
    int i;

    if(f == NULL || p == NULL){
        return;
    }

    fprintf(f, "JUGADOR: %02d\n", p->id_jugador);
    fprintf(f, "SALA: %02d\n", p->id_sala_actual);

    for(i = 0; p->objetos != NULL && p->objetos[i].id_objeto[0] != '\0'; i++){
        fprintf(f, "OBJETO: %s-%s\n", p->objetos[i].id_objeto, p->objetos[i].localizacion);
    }

    for(i = 0; p->conexiones != NULL && p->conexiones[i].id_conexion[0] != '\0'; i++){
        fprintf(f, "CONEXION: %s-%s\n", p->conexiones[i].id_conexion, p->conexiones[i].estado);
    }

    for(i = 0; p->puzles != NULL && p->puzles[i].id_puzle[0] != '\0'; i++){
        fprintf(f, "PUZLE: %s-%s\n", p->puzles[i].id_puzle, p->puzles[i].estado);
    }

    fprintf(f, "\n");
}

void guardar_partida(partida *p, int num_partidas, jugadores j){
    FILE *f;
    int i;
    (void)j;

    if(p == NULL || num_partidas <= 0){
        printf("No hay partidas para guardar.\n");
        return;
    }

    f = fopen("Partida.txt", "w");

    if(f == NULL){
        printf("Error al abrir el fichero Partida.txt para guardar.\n");
        return;
    }

    for(i = 0; i < num_partidas; i++){
        escribir_una_partida(f, &p[i]);
    }

    if(fflush(f) != 0){
        printf("Error al escribir los datos en Partida.txt\n");
        fclose(f);
        return;
    }

    fclose(f);
}

int crear_partida_nueva(partida **p, int *num_partidas, jug_vect *j, int id_jugador){
    int indice;
    int id_jugador_real;
    partida nueva;
    partida *tmp;

    if(p == NULL || num_partidas == NULL || j == NULL || id_jugador < 0 || id_jugador >= j->num_jug){
        return -1;
    }

    id_jugador_real = j->jug[id_jugador].Id_jugador;
    indice = buscar_indice_partida(*p, *num_partidas, id_jugador_real);

    if(indice != -1){
        liberar_partida_individual(&(*p)[indice]);
        if(!inicializar_partida_base(&(*p)[indice], id_jugador_real)){
            return -1;
        }
        sincronizar_inventario_desde_partida(&(*p)[indice], j, id_jugador);
        return indice;
    }

    memset(&nueva, 0, sizeof(nueva));
    if(!inicializar_partida_base(&nueva, id_jugador_real)){
        return -1;
    }

    tmp = (partida *)realloc(*p, (size_t)(*num_partidas + 1) * sizeof(partida));
    if(tmp == NULL){
        liberar_partida_individual(&nueva);
        return -1;
    }

    *p = tmp;
    (*p)[*num_partidas] = nueva;
    indice = *num_partidas;
    (*num_partidas)++;
    sincronizar_inventario_desde_partida(&(*p)[indice], j, id_jugador);
    return indice;
}

void sincronizar_inventario_desde_partida(partida *p, jug_vect *j, int id_jugador){
    char buffer[256];
    int i;
    int primero = 1;

    if(p == NULL || j == NULL || id_jugador < 0 || id_jugador >= j->num_jug){
        return;
    }

    buffer[0] = '\0';
    for(i = 0; p->objetos != NULL && p->objetos[i].id_objeto[0] != '\0'; i++){
        if(strcmp(p->objetos[i].localizacion, "Inventario") == 0 || strcmp(p->objetos[i].localizacion, "inventario") == 0){
            if(!primero){
                strcat(buffer, ",");
            }
            strcat(buffer, p->objetos[i].id_objeto);
            primero = 0;
        }
    }

    free(j->jug[id_jugador].Id_obj);
    j->jug[id_jugador].Id_obj = (char *)malloc(strlen(buffer) + 1);
    if(j->jug[id_jugador].Id_obj == NULL){
        return;
    }
    strcpy(j->jug[id_jugador].Id_obj, buffer);
}

void liberar_partida_individual(partida *p){
    if(p == NULL){
        return;
    }

    free(p->objetos);
    free(p->conexiones);
    free(p->puzles);
    p->objetos = NULL;
    p->conexiones = NULL;
    p->puzles = NULL;
    p->id_sala_anterior = -1;
}

void liberar_partidas(partida *p, int num_partidas){
    int i;

    if(p == NULL){
        return;
    }

    for(i = 0; i < num_partidas; i++){
        liberar_partida_individual(&p[i]);
    }

    free(p);
}

void eliminar_partida(partida **p, int *num_partidas, int id_jugador){
    int i, j;
    FILE *f;

    if(p == NULL || *p == NULL || num_partidas == NULL || *num_partidas <= 0){
        printf("No hay partidas cargadas para eliminar.\n");
        return;
    }

    i = buscar_indice_partida(*p, *num_partidas, id_jugador);
    if(i == -1){
        printf("No existe ninguna partida del jugador %d.\n", id_jugador);
        return;
    }

    liberar_partida_individual(&(*p)[i]);

    for(j = i; j < *num_partidas - 1; j++){
        (*p)[j] = (*p)[j + 1];
    }

    (*num_partidas)--;

    if(*num_partidas > 0){
        partida *tmp = (partida *)realloc(*p, (size_t)(*num_partidas) * sizeof(partida));
        if(tmp != NULL){
            *p = tmp;
        }
    }else{
        free(*p);
        *p = NULL;
    }

    f = fopen("Partida.txt", "w");
    if(f == NULL){
        printf("Error al abrir el fichero Partida.txt en funcion eliminar partida.\n");
        return;
    }

    for(j = 0; j < *num_partidas; j++){
        escribir_una_partida(f, &(*p)[j]);
    }
    fclose(f);
}