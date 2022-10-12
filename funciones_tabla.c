#include <stdlib.h>
#include <string.h>

#include "newio.h"
#include "hash.h"
#include "funciones_tabla.h"
#include "slist.h"

/* Funciones de la tabla hash */

/**
 * Retorna una copia de la palabra. 
 */
char *copiar_palabra(char *dato) {
  return stringdup(dato);
}

/**
 * Retorna 1 si son iguales, 0 si no. 
 */
int comparar_palabras(char *dato1, char *dato2) {
  if (strcmp(dato1, dato2) == 0)
    return 1;
  return 0;
}

/**
 * Libera la memoria de toda la lista en una posición de la tabla. 
 */
void destruir_palabras_en_lista(SList lista) {
  lista_destruir(lista, free);
}

/**
 * Retorna la posición en la tabla de una palabra según la función hash. 
 */
unsigned hashear_palabra(char *dato, unsigned total) {
  return hash_index(dato, total);
}
