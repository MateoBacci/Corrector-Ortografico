#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "funciones_tabla.h"
#include "hash.h"
#include "texman.h"
#include "slist.h"

unsigned hash_index (char *word, unsigned total) {
  unsigned  hash = 0;
  for (; *word != 0; ++word) 
    hash += *word + 31 * hash;
  return hash % total;
}

/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned capacidad, FuncionCopiadora copia,
                          FuncionComparadora comp, FuncionDestructora destr,
                          FuncionHash hash) {

  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash tabla = malloc(sizeof(struct _TablaHash));
  assert(tabla != NULL);
  tabla->palabras = malloc(sizeof(SList) * capacidad);
  assert(tabla->palabras != NULL);
  tabla->numElems = 0;
  tabla->capacidad = capacidad;
  tabla->factorCarga = 0;
  tabla->copia = copia;
  tabla->destr = destr;
  tabla->comp = comp;
  tabla->hash = hash;

  // Inicializamos las casillas con datos nulos.
  for (unsigned i = 0; i < capacidad; ++i) {
    tabla->palabras[i] = lista_crear();
  }

  return tabla;
}

void tablahash_destruir(TablaHash tabla) {
  
  // Destruir cada uno de los datos.
  for (unsigned i = 0; i < tabla->capacidad; i++)
    if (tabla->palabras[i] != NULL)
      tabla->destr(tabla->palabras[i]);

  // Liberar el arreglo de casillas y la tabla.
  free(tabla->palabras);
  free(tabla);
  return;
}

/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 */
void tablahash_insertar(TablaHash tabla, char *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned posicion = tabla->hash(dato, tabla->capacidad);

  tabla->palabras[posicion] = lista_agregar(tabla->palabras[posicion], dato);
  tabla->numElems++;
  tabla->factorCarga = (float)tabla->numElems / tabla->capacidad;

  //printf("%s\n", tabla->palabras[posicion]->dato);
}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
int tablahash_buscar(TablaHash tabla, char *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned posicion = tabla->hash(dato, tabla->capacidad);
  
  if (lista_buscar(tabla->palabras[posicion], dato) == NULL)
    return -1;
  
  return posicion;
}

void tablahash_redimensionar (TablaHash tabla) {
  SList *listaAux = tabla->palabras;
  tabla->capacidad *= 2;
  tabla->numElems = 0;
  tabla->palabras = malloc(sizeof(SList) * tabla->capacidad);

  for (unsigned i = 0; i < tabla->capacidad; ++i) {
    tabla->palabras[i] = lista_crear();
  }

  for (unsigned i = 0; i < tabla->capacidad / 2; i++) {
    SList nodoAux = listaAux[i];
    for (; nodoAux != NULL; nodoAux = nodoAux->sig) { 
      tablahash_insertar(tabla, nodoAux->dato);
    }
    lista_destruir(listaAux[i]);
  }
  free(listaAux);
}


TablaHash tablahash_armar (char *nombreArchivo) {
  FILE *archivo = fopen(nombreArchivo, "rb");
  assert(archivo != NULL);

  TablaHash tabla = tablahash_crear(2000, copiar_palabra, comparar_palabras, 
                                    destruir_palabra, hashear_palabra);

  while (!feof(archivo)) {
    char palabraAux[30];
    fscanf(archivo, "%s ", palabraAux);
    normalize_word(palabraAux);
    if (tabla->factorCarga >= 0.7) tablahash_redimensionar(tabla);
    tablahash_insertar(tabla, palabraAux);
  }

  fclose(archivo);

  return tabla;
}

int palabra_correcta (TablaHash tabla, char *palabra) {
  if (tablahash_buscar(tabla, palabra) == -1)
    return 0;
  return 1;
}