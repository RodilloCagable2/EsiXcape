#include "jugadores.h"
#include "objetos.h"
#include "menu.h"

int main() {
	jug_vect j;
	obj_vect o;
	int resultado;
	
	// Punto de entrada: cargar datos base, ejecutar el menu y guardar al salir.
	// Inicializar estructuras
	j.jug = NULL;
	j.num_jug = 0;
	memset(&o, 0, sizeof(obj_vect));
	
	// Cargar datos
	cargar_jugadores(&j);
	cargar_objetos(&o);
	
	// Ejecutar menú
	resultado = menu_principal(&j);
	
	// Guardar datos finales
	guardar_jugadores(&j);
	guardar_objetos(&o);
	
	// Liberar memoria
	if (j.jug != NULL) {
		int i;
		for (i = 0; i < j.num_jug; i++) {
			if (j.jug[i].Id_obj != NULL) {
				free(j.jug[i].Id_obj);
			}
		}
		free(j.jug);
	}
	
	return resultado;
}