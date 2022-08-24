#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "slist.h"

SList lista_crear() {
  return NULL;
}

void lista_destruir(SList lista) {
  while (lista != NULL) {
    SList nodoAux = lista;
    free(lista->dato);
    lista = lista->sig;
    free(nodoAux);
  }
}

SList lista_agregar(SList lista, char *dato) {
  int len = strlen(dato);

  SList nuevoNodo = malloc(sizeof(SNodo));
  assert(nuevoNodo != NULL);
  
  nuevoNodo->dato = malloc(sizeof(char) * len + 1);
  nuevoNodo->dato[len] = '\0';
  memcpy(nuevoNodo->dato, dato, len);
  
  nuevoNodo->sig = lista;
  return nuevoNodo;
}

char *lista_buscar (SList lista, char *dato) {
  if (lista == NULL) return NULL;

  if (strcmp(lista->dato, dato) == 0)
    return lista->dato;
  else 
    return lista_buscar (lista->sig, dato); 
}
