#include "jugadores.h"

//---> GESTIÓN DE MEMORIA: <---
void cargar_jugadores(jug_vect *j) {
	FILE *f_jug;
	char filename[] = "Jugadores.txt";
	char cad_linea[250];
	char id_obj_tmp[200];
	int i = 0, campo_jugadores = 0;
	j->num_jug = 0;
	j->jug = NULL;
	
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
	
	rewind(f_jug);
	
	//Contar número de jugadores
	while(fgets(cad_linea, sizeof(cad_linea), f_jug) != NULL) {
		if (cad_linea[0] != '\n' && cad_linea[0] != '\r') {
			j->num_jug++;
		}
	}
	
	if (file_size == 0 || j->num_jug == 0) {
		fclose(f_jug);
		return;
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
		do {
			if (fgets(cad_linea, sizeof(cad_linea), f_jug) == NULL) {
				printf("Error inesperado al leer los datos del jugador %d\n", i + 1);
				fclose(f_jug);
				exit(1);
			}
		} while (cad_linea[0] == '\n' || cad_linea[0] == '\r');
		
		id_obj_tmp[0] = '\0';
		campo_jugadores = sscanf(cad_linea, "%d-%21[^-]-%11[^-]-%9[^-]-%200[^\r\n]",
			&j->jug[i].Id_jugador,
			j->jug[i].Nomb_jugador,
			j->jug[i].Jugador,
			j->jug[i].Contrasena,
			id_obj_tmp);
		
		if (campo_jugadores < 4) {
			printf("Error en los datos del jugador %d\n", i + 1);
			while (i > 0) {
				i--;
				free(j->jug[i].Id_obj);
			}
			free(j->jug);
			fclose(f_jug);
			exit(1);
		}

		if (campo_jugadores == 4) {
			id_obj_tmp[0] = '\0';
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
}

void guardar_jugadores(jug_vect *j) {
	FILE *f_jug;
	char filename[] = "Jugadores.txt";
	int i;
	
	f_jug = fopen(filename, "w");
	if (f_jug == NULL) {
		printf("Error al abrir el archivo de jugadores");
		exit(1);
	}
	
	for (i = 0; i < j->num_jug; i++) {
		if (fprintf(f_jug, "%02d-%s-%s-%s-%s\n",
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
}