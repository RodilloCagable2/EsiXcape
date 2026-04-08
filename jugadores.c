#include "jugadores.h"

//---> GESTIÓN DE MEMORIA: <---
int cargar_jugadores(jug_vect *j) {
	FILE *f_jug;
	char filename[] = "Jugadores.txt";
	char default_jug[] = "00-Default Player-jugador0-AA000000-AAAA";
	char cad_linea[250];
	char id_obj_tmp[200];
	int i = 0, campo_jugadores = 0;
	j->num_jug = 0;
	
	//Abrir en modo lectura/escritura y crear si no existe
	f_jug = fopen(filename, "a+");
	
	if (f_jug == NULL) {
		printf("Error al abrir el archivo de jugadores");
		exit(1);
	}
	
	//Comprobar si el archivo está vacío
	if (fseek(f_jug, 0, SEEK_END) != 0) {
		printf("Error al posicionarse en el archivo de jugadores");
		fclose(f_jug);
		exit(1);
	}
	
	long file_size = ftell(f_jug);
	
	if (file_size == -1L) {
		printf("Error al obtener el tamaño del archivo de jugadores");
		fclose(f_jug);
		exit(1);
	}
	
	//Colocar un jugador por defecto si el archivo está vacío
	if (file_size == 0) {
		if (fprintf(f_jug, "%s\n", default_jug) < 0) {
			printf("Error al escribir el jugador por defecto");
			fclose(f_jug);
			exit(1);
		}
		fflush(f_jug);
	}
	
	rewind(f_jug);
	
	//Contar número de jugadores
	while(fgets(cad_linea, sizeof(cad_linea), f_jug) != NULL) {
		j->num_jug++;
	}
	
	if (j->num_jug == 0) {
		printf("No se encontraron jugadores en el archivo\n");
		fclose(f_jug);
		exit(1);
	}

	rewind(f_jug);
	
	//Reservar memoria para los jugadores
	j->jug = (jugadores*)malloc(j->num_jug * sizeof(jugadores));
	if (j->jug == NULL) {
		printf("Error al asignar memoria para los jugadores\n");
		fclose(f_jug);
		exit(1);
	}	
	
	//Cargar los datos de los jugadores
	for (i = 0; i < j->num_jug; i++) {
		if (fgets(cad_linea, sizeof(cad_linea), f_jug) == NULL) {
			printf("Error inesperado al leer los datos del jugador %d\n", i + 1);
			fclose(f_jug);
			exit(1);
		}
		
		campo_jugadores = sscanf(cad_linea, "%d-%21[^-]-%11[^-]-%9[^-]-%201[^\r\n]",
			&j->jug[i].Id_jugador,
			j->jug[i].Nomb_jugador,
			j->jug[i].Jugador,
			j->jug[i].Contrasena,
			id_obj_tmp);
		
		if (campo_jugadores != 5) {
			printf("Error en los datos del jugador %d\n", i + 1);
			while (i > 0) {
				i--;
				free(j->jug[i].Id_obj);
			}
			free(j->jug);
			fclose(f_jug);
			exit(1);
		}

		j->jug[i].Id_obj = (char*)malloc((strlen(id_obj_tmp) + 1) * sizeof(char));
		if (j->jug[i].Id_obj == NULL) {
			printf("Error al asignar memoria para Id_obj del jugador %d\n", i + 1);
			while (i > 0) {
				i--;
				free(j->jug[i].Id_obj);
			}
			free(j->jug);
			fclose(f_jug);
			exit(1);
		}
		strcpy(j->jug[i].Id_obj, id_obj_tmp);
	}
	
	fclose(f_jug);
	return 0;
}

int guardar_jugadores(jug_vect *j) {
	FILE *f_jug;
	char filename[] = "Jugadores.txt";
	int i;
	
	f_jug = fopen(filename, "w");
	if (f_jug == NULL) {
		printf("Error al abrir el archivo de jugadores");
		exit(1);
	}
	
	for (i = 0; i < j->num_jug; i++) {
		if (fprintf(f_jug, "%d-%s-%s-%s-%s\n",
			j->jug[i].Id_jugador,
			j->jug[i].Nomb_jugador,
			j->jug[i].Jugador,
			j->jug[i].Contrasena,
			j->jug[i].Id_obj != NULL ? j->jug[i].Id_obj : "") < 0) {
			printf("Error al escribir en el archivo de jugadores");
			fclose(f_jug);
			exit(1);		
		}
	}
	
	fclose(f_jug);
	return 0;
}