#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "funciones_tabla.h"
#include "hash.h"
#include "seatec.h"
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
TablaHash tablahash_crear(unsigned capacidad, FuncionCopiaTabla copia,
                          FuncionComparadoraTabla comp, FuncionDestructoraTabla destr,
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
void tablahash_insertar(TablaHash tabla, char *dato, int esDict) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned posicion = tabla->hash(dato, tabla->capacidad);
  SList verificador = tabla->palabras[posicion];
  tabla->palabras[posicion] = lista_agregar(tabla->palabras[posicion], dato,
          (FuncionCopiaLista)tabla->copia, (FuncionComparaLista)tabla->comp, esDict);
  
  if (tabla->palabras[posicion] != verificador) {   // Si son distintos es porque se agregó un nodo.
    tabla->numElems++;
    tabla->factorCarga = (float)tabla->numElems / tabla->capacidad;  
  }

  //printf("%s\n", tabla->palabras[posicion]->dato);
}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
int tablahash_buscar(TablaHash tabla, char *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned posicion = tabla->hash(dato, tabla->capacidad);
  
  if (lista_buscar(tabla->palabras[posicion], dato, (FuncionComparaLista)tabla->comp) == NULL)
    return -1;
  
  return posicion;
}

int requiere_redimensionar (TablaHash tabla) {
  return tabla->factorCarga >= 0.7;
}

void tablahash_redimensionar (TablaHash tabla, unsigned multiplo, int esDict) {
  SList *listaAux = tabla->palabras;
  tabla->capacidad *= multiplo;
  tabla->numElems = 0;
  tabla->palabras = malloc(sizeof(SList) * tabla->capacidad);

  for (unsigned i = 0; i < tabla->capacidad; ++i) {
    tabla->palabras[i] = lista_crear();
  }

  for (unsigned i = 0; i < tabla->capacidad / multiplo; i++) {
    SList nodoAux = listaAux[i];
    for (; nodoAux != NULL; nodoAux = nodoAux->sig) { 
      tablahash_insertar(tabla, nodoAux->dato, esDict);
    }
    tabla->destr(listaAux[i]);
  }
  free(listaAux);
}


TablaHash tablahash_armar_diccionario (char *nombreArchivo) {
  FILE *archivo = fopen(nombreArchivo, "rb");
  assert(archivo != NULL);

  TablaHash tabla = tablahash_crear(1000, copiar_palabra, comparar_palabras, 
                                    destruir_palabra, hashear_palabra);

  while (!feof(archivo)) {
    char palabraAux[30];
    fscanf(archivo, "%s ", palabraAux);
    for (int i = 0; palabraAux[i] != '\0'; i++) {
      palabraAux[i] = toupper(palabraAux[i]);
    }
    if (requiere_redimensionar(tabla)) tablahash_redimensionar(tabla, 2, 1);
    tablahash_insertar(tabla, palabraAux, 1);
  }

  fclose(archivo);

  return tabla;
}

int palabra_correcta (TablaHash tabla, char *palabra) {
  if (tablahash_buscar(tabla, palabra) == -1)
    return 0;
  return 1;
}