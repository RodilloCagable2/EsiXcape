#include "objetos.h"

//---> GESTIÓN DE MEMORIA: <---
void cargar_objetos(obj_vect *o) {
	FILE *f_obj;
	char filename[] = "Objetos.txt";
	char cad_linea[250];
	int i = 0, campo_objetos = 0;
	
	//Abrir en modo lectura/escritura y crear si no existe
	f_obj = fopen(filename, "a+");
	
	if (f_obj == NULL) {
		printf("Error al abrir el archivo de objetos");
		exit(1);
	}
	
	//Comprobar si el archivo está vacío
	if (fseek(f_obj, 0, SEEK_END) != 0) {
		printf("Error al posicionarse en el archivo de objetos");
		fclose(f_obj);
		exit(1);
	}
	
	long file_size = ftell(f_obj);
	
	if (file_size == -1L) {
		printf("Error al obtener el tamaño del archivo de objetos");
		fclose(f_obj);
		exit(1);
	}
	
	rewind(f_obj);
	
	//Cargar los datos de los objetos
	for (i = 0; i < 5; i++) {
		if (fgets(cad_linea, sizeof(cad_linea), f_obj) == NULL) {
			printf("Error inesperado al leer los datos del objeto %d\n", i + 1);
			fclose(f_obj);
			exit(1);
		}
		
		if (cad_linea[0] == '\n' || cad_linea[0] == '\r') {
			i--;
			continue;
		}
		
		campo_objetos = sscanf(cad_linea, "%5[^-]-%16[^-]-%51[^-]-%11[^\r\n]",
			o->obj[i].Id_obj,
			o->obj[i].Nomb_obj,
			o->obj[i].Describ,
			o->obj[i].Localiz);
		
		if (campo_objetos != 4) {
			printf("Error en los datos del objeto %d\n", i + 1);
			fclose(f_obj);
			exit(1);
		}
	}
	
	fclose(f_obj);
}

void guardar_objetos(obj_vect *o) {
	FILE *f_obj;
	char filename[] = "Objetos.txt";
	int i;
	
	f_obj = fopen(filename, "w");
	if (f_obj == NULL) {
		printf("Error al abrir el archivo de objetos");
		exit(1);
	}
	
	for (i = 0; i < 5; i++) {
		if (fprintf(f_obj, "%s-%s-%s-%s\n",
			o->obj[i].Id_obj,
			o->obj[i].Nomb_obj,
			o->obj[i].Describ,
			o->obj[i].Localiz) < 0) {
			printf("Error al escribir en el archivo de objetos");
			fclose(f_obj);
			exit(1);		
		}
	}
	
	fclose(f_obj);
}