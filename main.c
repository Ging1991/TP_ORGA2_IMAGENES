#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Firma de las funciones de enmascarar
int enmascarar_asm(unsigned char *a, unsigned char *b, unsigned char *mask, int cant);
int enmascarar_c(unsigned char *a, unsigned char *b, unsigned char *mask, int cant);

// Largo del archivo, es llenado por cargar vector
long largo;

// Dada una ruta, lee el archivo y lo devuelve como un vector
char* cargarVector(const char *ruta) {
	FILE *archivo;
	char *vector;

	// Abrimos el archivo en modo binario
	archivo = fopen(ruta, "rb");

	// Vamos al final del archivo
	fseek(archivo, 0, SEEK_END);

	// Obtenemos la posicion actual que marcara la cantidad de bytes que tiene el archivo
	largo = ftell(archivo);

	// Vamos de vuelta al comienzo del archivo
	rewind(archivo);

	// Declaramos nuestro vector con memoria sufieciente para cargar el archivo
	vector = (char *)malloc((largo+1)*sizeof(char));

	// Pasamos el contenido del archivo al vector
	fread(vector, largo, 1, archivo);

	// Cerramos el archivo y salimos
	fclose(archivo);
	return vector;
}

// Guarda un vector en un archivo de salida
int guardarVector(char *vector, int largo, const char *ruta) {
	FILE *archivo = fopen(ruta, "wb");
	fwrite(vector, sizeof(char), largo, archivo);
	fclose(archivo);
    return 0;
}

// Recorre los 3 vectores pisando A con B si Mask es FF
int enmascarar_c(unsigned char *a, unsigned char *b, unsigned char *mask, int cant) {
	int i;
	for(i = 0; i < cant; i++) {
		if (mask[i] != 0)
			a[i] = b[i];
	}
}


// Funcion principal
int main(int argc, char *argv[]) {
	
	// Valido cantidad de argumentos
	if (argc != 4) {
		printf("Error: Se ha ingresado una cantidad de parametros incorrecta: %d\n", argc-1);
		printf("Error: Deben ingresarse 3 parametros: imagen1, imagen2 y mascara");
		return 1;
	}

	char* ruta_imagen1 = argv[1];
	char* ruta_imagen2 = argv[2];
	char* ruta_mascara = argv[3];

	// Valido que sean rutas validas al cargar los vectores
	char *imagen1 = cargarVector(ruta_imagen1);
	if (largo == -1){
		printf("Error: La imagen 1 no correponde a la ruta de un archivo RGB valido: %s\n", ruta_imagen1);
		return 1;
	}

	char *imagen2 = cargarVector(ruta_imagen2);
	if (largo == -1){
		printf("Error: La imagen 2 no correponde a la ruta de un archivo RGB valido: %s\n", ruta_imagen2);
		return 1;
	}

	char *mascara = cargarVector(ruta_mascara);
	if (largo == -1){
		printf("Error: La imagen de mascara no correponde a la ruta de un archivo RGB valido: %s\n", ruta_mascara);
		return 1;
	}

	// Creo una copia del vector de imagen 1 para cada metodo
	int i;
	char imagen1_c[largo];
	for (i=0; i<largo; i++)
		imagen1_c[i] = imagen1[i];

	char imagen1_asm[largo];
	for (i=0; i<largo; i++)
		imagen1_asm[i] = imagen1[i];

	// Ejecuto ambos metodos viendo cuanto tardo cada uno
	clock_t tiempo_inicio, tiempo_final;
	double segundos_c;
	double segundos_asm;
	int ciclos = 10000;

	// ENMASCARAR EN ASM
	tiempo_inicio = clock();
	for (i = 0; i < ciclos; i++)
		enmascarar_asm(imagen1_asm, imagen2, mascara, largo);
	tiempo_final = clock();
	segundos_asm = (double)(tiempo_final - tiempo_inicio) / CLOCKS_PER_SEC;
	
	
	// ENMASCARAR EN C
	tiempo_inicio = clock();
	for (i = 0; i < ciclos; i++)
		enmascarar_c (imagen1_c, imagen2, mascara, largo);
	tiempo_final = clock();
	segundos_c = (double)(tiempo_final - tiempo_inicio) / CLOCKS_PER_SEC;
    
	// guardo ambas imagenes
	guardarVector(imagen1_asm, largo, "salida_asm.rgb");
	guardarVector(imagen1_c, largo, "salida_c.rgb");

	printf("Largo: %d, C: %f, ASM: %f\n", largo, segundos_c, segundos_asm);
	return 0;
}

