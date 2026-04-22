#include "menu_partida.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void limpiar_pantalla(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void pausar_pantalla(void) {
    char linea[8];

    printf("\nPulsa Enter para continuar...");
    fgets(linea, sizeof(linea), stdin);
}

static char *duplicar_cadena(const char *s) {
    char *copia;

    if (s == NULL) {
        return NULL;
    }

    copia = (char *)malloc(strlen(s) + 1);
    if (copia != NULL) {
        strcpy(copia, s);
    }

    return copia;
}

static int leer_entero_rango(const char *mensaje, int min, int max) {
    char linea[64];
    int valor;

    while (1) {
        printf("%s", mensaje);
        if (fgets(linea, sizeof(linea), stdin) == NULL) {
            continue;
        }
        if (sscanf(linea, "%d", &valor) == 1 && valor >= min && valor <= max) {
            return valor;
        }
        printf("\nEntrada no valida.\n");
    }
}

static void leer_texto_simple(const char *mensaje, char *destino, size_t tam) {
    printf("%s", mensaje);
    if (fgets(destino, (int)tam, stdin) != NULL) {
        destino[strcspn(destino, "\r\n")] = '\0';
    } else {
        destino[0] = '\0';
    }
}

static const char *nombre_sala_por_id(vector_sala *salas, int id_sala) {
    int i;

    for (i = 0; i < salas->numreg; i++) {
        if (salas->vctr_sala[i].id_sala == id_sala) {
            return salas->vctr_sala[i].nombr_sala;
        }
    }

    return "Desconocida";
}

static void imprimir_descripcion_sala_actual(partida *p) {
    vector_sala salas = {NULL, 0};
    int i;

    if (!cargar_salas(&salas)) {
        printf("No se pudieron cargar las salas.\n");
        return;
    }

    for (i = 0; i < salas.numreg; i++) {
        if (salas.vctr_sala[i].id_sala == p->id_sala_actual) {
            printf("=== %s ===\n", salas.vctr_sala[i].nombr_sala);
            printf("%s\n", salas.vctr_sala[i].desrc_sala);
            if (strcmp(salas.vctr_sala[i].type_sala, "SALIDA") == 0) {
                printf("\nFelicidades! Has llegado a la salida.\n");
            }
            break;
        }
    }

    free(salas.vctr_sala);
}

static void imprimir_cabecera_partida(partida *p) {
    vector_sala salas = {NULL, 0};

    if (!cargar_salas(&salas)) {
        printf("Sala actual: %02d\n\n", p->id_sala_actual);
        return;
    }

    printf("Sala: %s (%02d)\n\n", nombre_sala_por_id(&salas, p->id_sala_actual), p->id_sala_actual);
    free(salas.vctr_sala);
}

static int buscar_indice_objeto_partida(partida *p, const char *id_obj) {
    int i;

    for (i = 0; p->objetos[i].id_objeto[0] != '\0'; i++) {
        if (strcmp(p->objetos[i].id_objeto, id_obj) == 0) {
            return i;
        }
    }

    return -1;
}

static int buscar_indice_conexion_estado(partida *p, const char *id_conexion) {
    int i;

    for (i = 0; p->conexiones[i].id_conexion[0] != '\0'; i++) {
        if (strcmp(p->conexiones[i].id_conexion, id_conexion) == 0) {
            return i;
        }
    }

    return -1;
}

static int buscar_indice_puzle_estado(partida *p, const char *id_puzle) {
    int i;

    for (i = 0; p->puzles[i].id_puzle[0] != '\0'; i++) {
        if (strcmp(p->puzles[i].id_puzle, id_puzle) == 0) {
            return i;
        }
    }

    return -1;
}

static void examinar(partida *p) {
    obj_vect objs;
    vector_conex conexiones_base = {NULL, 0};
    vector_sala salas = {NULL, 0};
    int i, j;
    int hay_objetos = 0;
    int hay_salidas = 0;

    limpiar_pantalla();

    cargar_objetos(&objs);
    if (!cargar_conex(&conexiones_base) || !cargar_salas(&salas)) {
        printf("No se pudo examinar la sala.\n");
        free(conexiones_base.conexvec);
        free(salas.vctr_sala);
        pausar_pantalla();
        return;
    }

    printf("=== EXAMINAR SALA ===\n");
    printf("Sala actual: %s\n\n", nombre_sala_por_id(&salas, p->id_sala_actual));

    printf("Objetos en la sala:\n");
    for (i = 0; p->objetos[i].id_objeto[0] != '\0'; i++) {
        if (atoi(p->objetos[i].localizacion) == p->id_sala_actual) {
            for (j = 0; j < 5; j++) {
                if (strcmp(objs.obj[j].Id_obj, p->objetos[i].id_objeto) == 0) {
                    printf("- %s: %s\n", objs.obj[j].Nomb_obj, objs.obj[j].Describ);
                    hay_objetos = 1;
                    break;
                }
            }
        }
    }
    if (!hay_objetos) {
        printf("No hay objetos en esta sala.\n");
    }

    printf("\nSalidas disponibles:\n");
    for (i = 0; i < conexiones_base.conexreg; i++) {
        if (conexiones_base.conexvec[i].id_origen == p->id_sala_actual) {
            j = buscar_indice_conexion_estado(p, conexiones_base.conexvec[i].id_conexion);
            if (j != -1) {
                printf("- Sala %02d (%s): %s",
                       conexiones_base.conexvec[i].id_destino,
                       nombre_sala_por_id(&salas, conexiones_base.conexvec[i].id_destino),
                       p->conexiones[j].estado);
                if (strcmp(p->conexiones[j].estado, "Bloqueada") == 0) {
                    printf(" (Condicion: %s)", conexiones_base.conexvec[i].condicion);
                }
                printf("\n");
                hay_salidas = 1;
            }
        }
    }
    if (!hay_salidas) {
        printf("No hay salidas desde esta sala.\n");
    }

    free(conexiones_base.conexvec);
    free(salas.vctr_sala);
    pausar_pantalla();
}

// Anterior aunque no exista una conexión explícita de vuelta.
static void entrar_en_otra_sala(partida *p) {
    vector_conex conexiones_base = {NULL, 0};
    vector_sala salas = {NULL, 0};
    int destino;
    int i, j;
    int encontrada = 0;

    limpiar_pantalla();

    if (!cargar_conex(&conexiones_base) || !cargar_salas(&salas)) {
        printf("No se pudieron cargar las conexiones.\n");
        free(conexiones_base.conexvec);
        free(salas.vctr_sala);
        pausar_pantalla();
        return;
    }

    printf("=== ENTRAR EN OTRA SALA ===\n");
    printf("Sala actual: %s (%02d)\n\n", nombre_sala_por_id(&salas, p->id_sala_actual), p->id_sala_actual);
    printf("Salas accesibles desde aqui:\n");

    for (i = 0; i < conexiones_base.conexreg; i++) {
        if (conexiones_base.conexvec[i].id_origen == p->id_sala_actual) {
            j = buscar_indice_conexion_estado(p, conexiones_base.conexvec[i].id_conexion);
            if (j != -1) {
                printf("- %02d -> %s [%s]",
                       conexiones_base.conexvec[i].id_destino,
                       nombre_sala_por_id(&salas, conexiones_base.conexvec[i].id_destino),
                       p->conexiones[j].estado);
                if (strcmp(p->conexiones[j].estado, "Bloqueada") == 0) {
                    printf(" (Condicion: %s)", conexiones_base.conexvec[i].condicion);
                }
                printf("\n");
            }
        }
    }

    if (p->id_sala_anterior > 0) {
        printf("\n0. Volver a la sala anterior (%s)\n", nombre_sala_por_id(&salas, p->id_sala_anterior));
    } else {
        printf("\n0. Cancelar\n");
    }

    destino = leer_entero_rango("\nIntroduce el id de la sala destino (o 0): ", 0, 99);

    if (destino == 0) {
        if (p->id_sala_anterior > 0) {
            int sala_temporal = p->id_sala_actual;
            p->id_sala_actual = p->id_sala_anterior;
            p->id_sala_anterior = sala_temporal;
            printf("\nHas vuelto a la sala anterior.\n");
        } else {
            printf("\nOperacion cancelada.\n");
        }
        free(conexiones_base.conexvec);
        free(salas.vctr_sala);
        pausar_pantalla();
        return;
    }

    for (i = 0; i < conexiones_base.conexreg; i++) {
        if (conexiones_base.conexvec[i].id_origen == p->id_sala_actual && conexiones_base.conexvec[i].id_destino == destino) {
            encontrada = 1;
            j = buscar_indice_conexion_estado(p, conexiones_base.conexvec[i].id_conexion);
            if (j != -1) {
                if (strcmp(p->conexiones[j].estado, "Activa") == 0 || strcmp(p->conexiones[j].estado, "ABIERTA") == 0) {
                    p->id_sala_anterior = p->id_sala_actual;
                    p->id_sala_actual = destino;
                    printf("\nHas entrado en la sala %02d.\n", destino);
                } else {
                    printf("\nLa conexion está bloqueada. Necesitas: %s\n", conexiones_base.conexvec[i].condicion);
                }
            }
            break;
        }
    }

    if (!encontrada) {
        printf("\nNo hay conexion directa a esa sala.\n");
    }

    free(conexiones_base.conexvec);
    free(salas.vctr_sala);
    pausar_pantalla();
}

static void coger_objeto(partida *p, jug_vect *j, int id_jugador) {
    obj_vect objs;
    int indices[5];
    int count = 0;
    int i, k;
    int sel;

    limpiar_pantalla();

    cargar_objetos(&objs);

    printf("=== COGER OBJETO ===\n\n");
    printf("Objetos disponibles en la sala:\n");

    for (i = 0; p->objetos[i].id_objeto[0] != '\0'; i++) {
        if (atoi(p->objetos[i].localizacion) == p->id_sala_actual) {
            for (k = 0; k < 5; k++) {
                if (strcmp(objs.obj[k].Id_obj, p->objetos[i].id_objeto) == 0) {
                    printf("%d. %s\n", count + 1, objs.obj[k].Nomb_obj);
                    indices[count] = i;
                    count++;
                    break;
                }
            }
        }
    }

    if (count == 0) {
        printf("No hay objetos en esta sala.\n");
        pausar_pantalla();
        return;
    }

    printf("0. Cancelar\n\n");
    sel = leer_entero_rango("Selecciona el objeto a coger: ", 0, count);
    if (sel == 0) {
        printf("Operacion cancelada.\n");
        pausar_pantalla();
        return;
    }

    strcpy(p->objetos[indices[sel - 1]].localizacion, "Inventario");
    sincronizar_inventario_desde_partida(p, j, id_jugador);

    for (k = 0; k < 5; k++) {
        if (strcmp(objs.obj[k].Id_obj, p->objetos[indices[sel - 1]].id_objeto) == 0) {
            printf("\nHas cogido %s.\n", objs.obj[k].Nomb_obj);
            break;
        }
    }

    pausar_pantalla();
}

static void soltar_objeto(partida *p, jug_vect *j, int id_jugador) {
    obj_vect objs;
    char *inv;
    char *token;
    char ids[10][5];
    int count = 0;
    int sel;
    int indice_objeto;
    int i;

    limpiar_pantalla();
    printf("=== SOLTAR OBJETO ===\n\n");

    if (j->jug[id_jugador].Id_obj == NULL || j->jug[id_jugador].Id_obj[0] == '\0') {
        printf("Tu inventario está vacio.\n");
        pausar_pantalla();
        return;
    }

    cargar_objetos(&objs);

    inv = duplicar_cadena(j->jug[id_jugador].Id_obj);
    if (inv == NULL) {
        pausar_pantalla();
        return;
    }

    printf("Objetos en tu inventario:\n");
    token = strtok(inv, ",");
    while (token != NULL && count < 10) {
        strcpy(ids[count], token);
        for (i = 0; i < 5; i++) {
            if (strcmp(objs.obj[i].Id_obj, token) == 0) {
                printf("%d. %s\n", count + 1, objs.obj[i].Nomb_obj);
                break;
            }
        }
        count++;
        token = strtok(NULL, ",");
    }
    free(inv);

    if (count == 0) {
        printf("Tu inventario está vacio.\n");
        pausar_pantalla();
        return;
    }

    printf("0. Cancelar\n\n");
    sel = leer_entero_rango("Selecciona el objeto a soltar: ", 0, count);
    if (sel == 0) {
        printf("Operacion cancelada.\n");
        pausar_pantalla();
        return;
    }

    indice_objeto = buscar_indice_objeto_partida(p, ids[sel - 1]);
    if (indice_objeto == -1) {
        printf("No se ha encontrado ese objeto en la partida.\n");
        pausar_pantalla();
        return;
    }

    sprintf(p->objetos[indice_objeto].localizacion, "%02d", p->id_sala_actual);
    sincronizar_inventario_desde_partida(p, j, id_jugador);
    printf("\nObjeto soltado en la sala.\n");
    pausar_pantalla();
}

static void mostrar_inventario(jug_vect *j, int id_jugador) {
    obj_vect objs;
    char *inv;
    char *token;
    int i;

    limpiar_pantalla();
    printf("=== INVENTARIO ===\n\n");
    printf("Inventario de %s:\n", j->jug[id_jugador].Jugador);

    if (j->jug[id_jugador].Id_obj == NULL || j->jug[id_jugador].Id_obj[0] == '\0') {
        printf("Vacio.\n");
        pausar_pantalla();
        return;
    }

    cargar_objetos(&objs);

    inv = duplicar_cadena(j->jug[id_jugador].Id_obj);
    if (inv == NULL) {
        pausar_pantalla();
        return;
    }

    token = strtok(inv, ",");
    while (token != NULL) {
        for (i = 0; i < 5; i++) {
            if (strcmp(objs.obj[i].Id_obj, token) == 0) {
                printf("- %s: %s\n", objs.obj[i].Nomb_obj, objs.obj[i].Describ);
                break;
            }
        }
        token = strtok(NULL, ",");
    }

    free(inv);
    pausar_pantalla();
}

static void usar_objeto(partida *p, jug_vect *j, int id_jugador) {
    obj_vect objs;
    vector_conex conexiones_base = {NULL, 0};
    char *inv;
    char *token;
    char ids[10][5];
    int count = 0;
    int sel;
    int i, k;
    int usado = 0;

    limpiar_pantalla();
    printf("=== USAR OBJETO ===\n\n");

    if (j->jug[id_jugador].Id_obj == NULL || j->jug[id_jugador].Id_obj[0] == '\0') {
        printf("Tu inventario está vacio.\n");
        pausar_pantalla();
        return;
    }

    cargar_objetos(&objs);
    if (!cargar_conex(&conexiones_base)) {
        printf("No se pudieron cargar los datos necesarios.\n");
        free(conexiones_base.conexvec);
        pausar_pantalla();
        return;
    }

    inv = duplicar_cadena(j->jug[id_jugador].Id_obj);
    if (inv == NULL) {
        free(conexiones_base.conexvec);
        pausar_pantalla();
        return;
    }

    printf("Objetos en tu inventario:\n");
    token = strtok(inv, ",");
    while (token != NULL && count < 10) {
        strcpy(ids[count], token);
        for (i = 0; i < 5; i++) {
            if (strcmp(objs.obj[i].Id_obj, token) == 0) {
                printf("%d. %s\n", count + 1, objs.obj[i].Nomb_obj);
                break;
            }
        }
        count++;
        token = strtok(NULL, ",");
    }
    free(inv);

    printf("0. Cancelar\n\n");
    sel = leer_entero_rango("Selecciona el objeto a usar: ", 0, count);
    if (sel == 0) {
        printf("Operacion cancelada.\n");
        free(conexiones_base.conexvec);
        pausar_pantalla();
        return;
    }

    for (i = 0; i < conexiones_base.conexreg; i++) {
        if (conexiones_base.conexvec[i].id_origen == p->id_sala_actual && strcmp(conexiones_base.conexvec[i].condicion, ids[sel - 1]) == 0) {
            k = buscar_indice_conexion_estado(p, conexiones_base.conexvec[i].id_conexion);
            if (k != -1 && strcmp(p->conexiones[k].estado, "Bloqueada") == 0) {
                strcpy(p->conexiones[k].estado, "ABIERTA");
                printf("Has usado el objeto y has abierto la conexion hacia la sala %02d.\n",
                       conexiones_base.conexvec[i].id_destino);
                usado = 1;
            }
        }
    }

    if (!usado) {
        printf("No se puede usar ese objeto aqui.\n");
    }

    free(conexiones_base.conexvec);
    pausar_pantalla();
}

static void resolver_puzle(partida *p) {
    puzle *puzles = NULL;
    vector_conex conexiones_base = {NULL, 0};
    int i;
    int indice_estado;
    int encontrado = -1;
    char respuesta[64];
    int desbloqueado = 0;

    limpiar_pantalla();
    printf("=== RESOLVER PUZLE / INTRODUCIR CÓDIGO ===\n\n");

    cargar_puzles(&puzles);
    if (puzles == NULL || !cargar_conex(&conexiones_base)) {
        printf("No se pudo cargar la información del puzle.\n");
        free(puzles);
        free(conexiones_base.conexvec);
        pausar_pantalla();
        return;
    }

    for (i = 0; puzles[i].id_puzle[0] != '\0'; i++) {
        if (puzles[i].id_sala == p->id_sala_actual) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("No hay puzle en esta sala.\n");
        free(puzles);
        free(conexiones_base.conexvec);
        pausar_pantalla();
        return;
    }

    indice_estado = buscar_indice_puzle_estado(p, puzles[encontrado].id_puzle);
    if (indice_estado != -1 && strcmp(p->puzles[indice_estado].estado, "Resuelto") == 0) {
        printf("Este puzle ya está resuelto.\n");
        free(puzles);
        free(conexiones_base.conexvec);
        pausar_pantalla();
        return;
    }

    printf("Puzle: %s\n", puzles[encontrado].nombre);
    printf("Descripción: %s\n", puzles[encontrado].descripcion);
    printf("Tipo: %s\n\n", puzles[encontrado].tipo);
    leer_texto_simple("Introduce la solución: ", respuesta, sizeof(respuesta));

    if (comprobar_solucion_puzle(puzles, puzles[encontrado].id_puzle, respuesta) == 1) {
        if (indice_estado != -1) {
            strcpy(p->puzles[indice_estado].estado, "Resuelto");
        }
        printf("\n¡Correcto!\n");

        for (i = 0; i < conexiones_base.conexreg; i++) {
            if (conexiones_base.conexvec[i].id_origen == p->id_sala_actual &&
                strcmp(conexiones_base.conexvec[i].condicion, puzles[encontrado].id_puzle) == 0) {
                int indice_conexion = buscar_indice_conexion_estado(p, conexiones_base.conexvec[i].id_conexion);
                if (indice_conexion != -1) {
                    strcpy(p->conexiones[indice_conexion].estado, "ABIERTA");
                    desbloqueado = 1;
                }
            }
        }

        if (desbloqueado) {
            printf("Se han desbloqueado las conexiones asociadas a este puzle.\n");
        }
    } else {
        printf("\nRespuesta incorrecta.\n");
    }

    free(puzles);
    free(conexiones_base.conexvec);
    pausar_pantalla();
}

static void guardar_partida_menu(partida *partidas, int num_partidas, jug_vect *j, int indice_partida, int id_jugador) {
    limpiar_pantalla();
    
    if(partidas == NULL || indice_partida < 0 || indice_partida >= num_partidas){
        printf("Error: No hay partida valida para guardar.\n");
        pausar_pantalla();
        return;
    }
    
    sincronizar_inventario_desde_partida(&partidas[indice_partida], j, id_jugador);
    guardar_jugadores(j);
    guardar_partida(partidas, num_partidas, j->jug[id_jugador]);
    printf("Partida guardada correctamente.\n");
    pausar_pantalla();
}

int mostrar_partida(partida *partidas, int num_partidas, jug_vect *j, int indice_partida, int id_jugador) {
    int opcion;
    int mostrar_descripcion = 0;
    partida *p;

    if (partidas == NULL || j == NULL || indice_partida < 0 || indice_partida >= num_partidas ||
        id_jugador < 0 || id_jugador >= j->num_jug) {
        return 0;
    }

    p = &partidas[indice_partida];
    sincronizar_inventario_desde_partida(p, j, id_jugador);

    while (1) {
        limpiar_pantalla();
        imprimir_cabecera_partida(p);

        if (mostrar_descripcion) {
            imprimir_descripcion_sala_actual(p);
            printf("\n");
        }

        printf("1. Describir sala\n");
        printf("2. Examinar (objetos y salidas)\n");
        printf("3. Entrar en otra sala\n");
        printf("4. Coger objeto\n");
        printf("5. Soltar objeto\n");
        printf("6. Inventario\n");
        printf("7. Usar objeto\n");
        printf("8. Resolver puzle / introducir código\n");
        printf("9. Guardar partida\n");
        printf("10. Volver al menu principal\n");
        printf("0. Salir del juego\n\n");

        opcion = leer_entero_rango("Selecciona una opcion: ", 0, 10);

        switch (opcion) {
            case 0:
                sincronizar_inventario_desde_partida(p, j, id_jugador);
                guardar_jugadores(j);
                guardar_partida(partidas, num_partidas, j->jug[id_jugador]);
                return -1;

            case 1:
                mostrar_descripcion = 1;
                break;

            case 2:
                mostrar_descripcion = 0;
                examinar(p);
                break;

            case 3:
                mostrar_descripcion = 0;
                entrar_en_otra_sala(p);
                break;

            case 4:
                mostrar_descripcion = 0;
                coger_objeto(p, j, id_jugador);
                break;

            case 5:
                mostrar_descripcion = 0;
                soltar_objeto(p, j, id_jugador);
                break;

            case 6:
                mostrar_descripcion = 0;
                mostrar_inventario(j, id_jugador);
                break;

            case 7:
                mostrar_descripcion = 0;
                usar_objeto(p, j, id_jugador);
                break;

            case 8:
                mostrar_descripcion = 0;
                resolver_puzle(p);
                break;

            case 9:
                mostrar_descripcion = 0;
                guardar_partida_menu(partidas, num_partidas, j, indice_partida, id_jugador);
                break;

            case 10:
                sincronizar_inventario_desde_partida(p, j, id_jugador);
                guardar_jugadores(j);
                return 0;
        }
    }
}
