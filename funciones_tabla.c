#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "funciones_tabla.h"
#include "slist.h"

char *copiar_palabra (char *dato) {
  return strdup(dato);
}

int comparar_palabras (char *dato1, char *dato2) {
  return strcmp(dato1, dato2);
}

void destruir_palabra (SList lista) {
  lista_destruir(lista);
}

unsigned hashear_palabra (char *dato, unsigned total) {
  return hash_index(dato, total);
}
