#include "jugadores.h"
#include "objetos.h"
#include "menu.h"
#include "partida.h"
#include "menu_partida.h"

static void limpiar_buffer_entrada(void) {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {
		;
	}
}

static int buscar_usuario(jug_vect *j, const char *usuario) {
	int i;
	for (i = 0; i < j->num_jug; i++) {
		if (strcmp(j->jug[i].Jugador, usuario) == 0) {
			return i;
		}
	}
	return -1;
}

static int obtener_siguiente_id(jug_vect *j) {
	int i;
	int max_id = 0;
	for (i = 0; i < j->num_jug; i++) {
		if (j->jug[i].Id_jugador > max_id) {
			max_id = j->jug[i].Id_jugador;
		}
	}
	return max_id + 1;
}

static int registrar_jugador(jug_vect *j, const char *nombre_completo, const char *usuario, const char *contrasena) {
	jugadores *tmp;
	int idx;

	tmp = (jugadores *)realloc(j->jug, (j->num_jug + 1) * sizeof(jugadores));
	if (tmp == NULL) {
		printf("No se pudo registrar el jugador.\n");
		return -1;
	}

	j->jug = tmp;
	idx = j->num_jug;
	j->jug[idx].Id_jugador = obtener_siguiente_id(j);
	strncpy(j->jug[idx].Nomb_jugador, nombre_completo, sizeof(j->jug[idx].Nomb_jugador) - 1);
	j->jug[idx].Nomb_jugador[sizeof(j->jug[idx].Nomb_jugador) - 1] = '\0';
	strncpy(j->jug[idx].Jugador, usuario, sizeof(j->jug[idx].Jugador) - 1);
	j->jug[idx].Jugador[sizeof(j->jug[idx].Jugador) - 1] = '\0';
	strncpy(j->jug[idx].Contrasena, contrasena, sizeof(j->jug[idx].Contrasena) - 1);
	j->jug[idx].Contrasena[sizeof(j->jug[idx].Contrasena) - 1] = '\0';
	j->jug[idx].Id_obj = (char *)malloc(1);
	if (j->jug[idx].Id_obj == NULL) {
		return -1;
	}
	j->jug[idx].Id_obj[0] = '\0';
	j->num_jug++;

	return idx;
}

static int autenticar_o_registrar(jug_vect *j) {
	char nombre_completo[21];
	char usuario[11];
	char contrasena[9];
	char opcion;
	int idx;

	while (1) {
		system("cls");
		printf("<<< ACCESO A ESI-ESCAPE >>>\n\n");
		printf("Usuario: ");
		if (scanf("%11s", usuario) != 1) {
			limpiar_buffer_entrada();
			continue;
		}
		printf("Contrasena: ");
		if (scanf("%9s", contrasena) != 1) {
			limpiar_buffer_entrada();
			continue;
		}
		limpiar_buffer_entrada();

		if (strlen(usuario) > 10 || strlen(contrasena) > 8) {
			printf("\nUsuario o contrasena demasiado largos.\n");
			printf("Maximos: usuario 10, contrasena 8 caracteres.\n");
			printf("Pulsa Enter para continuar...");
			getchar();
			continue;
		}

		idx = buscar_usuario(j, usuario);
		if (idx != -1 && strcmp(j->jug[idx].Contrasena, contrasena) == 0) {
			return idx;
		}

		printf("\nUsuario o contrasena incorrectos.\n");
		printf("Deseas registrarte con un nuevo usuario? (S/N): ");
		if (scanf(" %c", &opcion) != 1) {
			limpiar_buffer_entrada();
			continue;
		}
		limpiar_buffer_entrada();

		if (opcion == 'S' || opcion == 's') {
			system("cls");
			printf("<<< REGISTRO DE NUEVO JUGADOR >>>\n\n");
			printf("Nombre completo (Nomb_jugador): ");
			if (fgets(nombre_completo, sizeof(nombre_completo), stdin) == NULL) {
				limpiar_buffer_entrada();
				continue;
			}
			
			nombre_completo[strcspn(nombre_completo, "\r\n")] = '\0';
			
			if (strlen(nombre_completo) == 0 || strlen(nombre_completo) > 20) {
				printf("\nNombre invalido. Debe tener entre 1 y 20 caracteres.\n");
				printf("Pulsa Enter para continuar...");
				getchar();
				continue;
			}

			printf("Nombre jugador (Jugador): ");
			if (fgets(usuario, sizeof(usuario), stdin) == NULL) {
				limpiar_buffer_entrada();
				continue;
			}
			usuario[strcspn(usuario, "\r\n")] = '\0';

			if (strlen(usuario) == 0 || strlen(usuario) > 10) {
				printf("\nUsuario invalido. Debe tener entre 1 y 10 caracteres.\n");
				printf("Pulsa Enter para continuar...");
				getchar();
				continue;
			}

			if (buscar_usuario(j, usuario) != -1) {
				printf("\nEse usuario ya existe. Elige otro.\n");
				printf("Pulsa Enter para continuar...");
				getchar();
				continue;
			}

			printf("Contrasena (Contrasena): ");
			if (fgets(contrasena, sizeof(contrasena), stdin) == NULL) {
				limpiar_buffer_entrada();
				continue;
			}
			contrasena[strcspn(contrasena, "\r\n")] = '\0';

			if (strlen(contrasena) == 0 || strlen(contrasena) > 8) {
				printf("\nContrasena invalida. Debe tener entre 1 y 8 caracteres.\n");
				printf("Pulsa Enter para continuar...");
				getchar();
				continue;
			}

			idx = registrar_jugador(j, nombre_completo, usuario, contrasena);
			if (idx == -1) {
				printf("\nNo se pudo completar el registro.\n");
				printf("Pulsa Enter para continuar...");
				getchar();
				continue;
			}

			guardar_jugadores(j);
			printf("\nRegistro completado. Bienvenido/a a ESI-ESCAPE, %s.\n", nombre_completo);
			printf("Pulsa Enter para continuar...");
			getchar();
			return idx;
		}
	}
}

int menu_principal(jug_vect *j) {
	int op;
	partida *partidas = NULL;
	int num_partidas = 0;
	int indice_partida = -1;
	int id_jugador;
	int resultado;
	
	id_jugador = autenticar_o_registrar(j);
	system("cls");
	printf("Bienvenido/a, %s.\n", j->jug[id_jugador].Jugador);
	printf("Pulsa Enter para acceder al menu principal...");
	getchar();

	// Cargar partidas existentes
	num_partidas = cargar_partida(&partidas);
	
	do {
		do {
			system("cls");
			printf("\n<<<ESI-ESCAPE>>>\n\n");
			Sleep(1000);
			printf("1. Nueva partida\n");
			printf("2. Cargar partida\n");
			printf("3. Salir\n\n");
			if (scanf("%d", &op) != 1) {
				limpiar_buffer_entrada();
				op = 0;
				continue;
			}
			limpiar_buffer_entrada();
		}while (op < 1 || op > 3);
		indice_partida = -1;
		
		switch (op) {
			case 1:
				// Nueva partida: crea el estado inicial y entra al menu jugable.
				indice_partida = crear_partida_nueva(&partidas, &num_partidas, j, id_jugador);
				if (indice_partida != -1) {
					resultado = mostrar_partida(partidas, num_partidas, j, indice_partida, id_jugador);
					if (resultado == -1) {
						// Usuario quiere salir del programa completamente
						guardar_jugadores(j);
						guardar_partida(partidas, num_partidas, j->jug[id_jugador]);
						liberar_partidas(partidas, num_partidas);
						exit(0);
					}
				}
				break;
			
			case 2:
				// Cargar partida: reutiliza la partida ya guardada para ese jugador.
				indice_partida = buscar_indice_partida(partidas, num_partidas, j->jug[id_jugador].Id_jugador);
				if (indice_partida != -1) {
					resultado = mostrar_partida(partidas, num_partidas, j, indice_partida, id_jugador);
					if (resultado == -1) {
						// Usuario quiere salir del programa completamente
						guardar_jugadores(j);
						guardar_partida(partidas, num_partidas, j->jug[id_jugador]);
						liberar_partidas(partidas, num_partidas);
						exit(0);
					}
				} else {
					printf("\nNo hay partida guardada.\n");
					printf("Pulsa Enter para volver al menu...\n");
					getchar();
				}
				break;
				
			case 3:
				printf("\nHasta la proxima!\n");
				break;
				
			default:
				printf("Seleccione una opcion valida: ");
				break;
		}
	}while (op != 3);
	
	// Guardar datos finales
	guardar_jugadores(j);
	if (partidas != NULL) {
		guardar_partida(partidas, num_partidas, j->jug[id_jugador]);
		liberar_partidas(partidas, num_partidas);
	}
	
	return 0;
}