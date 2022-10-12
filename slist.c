#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "slist.h"

/**
 * Crea una lista vacía.
 */
SList lista_crear() {
  return NULL;
}

/**
 * Destruye una lista. 
 */
void lista_destruir(SList lista, FuncionDestruyeLista dest) {
  SList nodoAux;
  while (lista != NULL) {
    nodoAux = lista;
    lista = lista->sig;
    dest(nodoAux->dato);
    free(nodoAux);
  }
}

/**
 * Agrega un nodo al inicio de la lista. 
 */
SList lista_agregar(SList lista, void *dato, FuncionCopiaLista copiar) {
  SList nuevoNodo = malloc(sizeof(SNodo));
  assert(nuevoNodo != NULL);
  nuevoNodo->dato = copiar(dato);
  nuevoNodo->sig = lista;
  return nuevoNodo;
}

/**
 * Busca una elemnto en una lista. Si lo encuentra, lo retorna, si no NULL. 
 */
void *lista_buscar(SList lista, void *dato, FuncionComparaLista comp) {
  if (lista == NULL)
    return NULL;

  if (comp(lista->dato, dato))
    return lista->dato;

  return lista_buscar(lista->sig, dato, comp);
}
