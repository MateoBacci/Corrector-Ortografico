#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "newio.h"
#include "funciones_tabla.h"
#include "hash.h"
#include "seatec.h"
#include "slist.h"

/**
 * Funcion de hash para strings propuesta por Kernighan & Ritchie en "The C
 * Programming Language (Second Ed.)".
 */
unsigned hash_index(char *word, unsigned total) {
  unsigned hash = 0;
  for (; *word != 0; ++word)
    hash += *word + 31 * hash;
  return hash % total;
}

/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned capacidad, FuncionCopiaTabla copia,
                          FuncionComparadoraTabla comp,
                          FuncionDestructoraTabla destr, FuncionHash hash) {

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
  // Destruye cada uno de los datos.
  for (unsigned i = 0; i < tabla->capacidad; i++)
    if (tabla->palabras[i] != NULL) {
      tabla->destr(tabla->palabras[i]);
    }
  // Libera el arreglo de casillas y la tabla.
  free(tabla->palabras);
  free(tabla);
  return;
}

/**
 * Inserta un dato en la tabla. En caso de colisión utiliza encadenamiento con
 * listas enlazadas simples.
 */
void tablahash_insertar(TablaHash tabla, char *dato) {
  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned posicion = tabla->hash(dato, tabla->capacidad);
  tabla->palabras[posicion] =
      lista_agregar(tabla->palabras[posicion], dato,
                    (FuncionCopiaLista) tabla->copia);
  tabla->numElems++;
  tabla->factorCarga = (float) tabla->numElems / tabla->capacidad;
}

/**
 * Busca el dato en la tabla, si lo encuentra retorna su posición, -1 si no.
 */
int tablahash_buscar(TablaHash tabla, char *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned posicion = tabla->hash(dato, tabla->capacidad);

  // Busca el dato en la posición obtenida.
  if (lista_buscar
      (tabla->palabras[posicion], dato,
       (FuncionComparaLista) tabla->comp) == NULL)
    return -1;

  return posicion;
}

/* Verifica si la tabla superó el límite de su factor de carga. */
int requiere_redimensionar(TablaHash tabla) {
  return tabla->factorCarga >= 0.6;
}

/* Redimensiona la tabla */
void tablahash_redimensionar(TablaHash tabla) {
  SList *listaAux = tabla->palabras;

  tabla->capacidad *= 2;
  tabla->numElems = 0;
  tabla->palabras = malloc(sizeof(SList) * tabla->capacidad);

  for (unsigned i = 0; i < tabla->capacidad; i++) 
    tabla->palabras[i] = lista_crear();

  unsigned i = 0;
  for (; i < tabla->capacidad / 2; i++) {
    SList nodoAux = listaAux[i];
    for (; nodoAux != NULL; nodoAux = nodoAux->sig) 
      tablahash_insertar(tabla, nodoAux->dato);
    
    lista_destruir(listaAux[i], (FuncionDestruyeLista) free);
  }

  free(listaAux);
}


/* Guarda las palabras del archivo tomado como diccionario en una tabla hash. */
TablaHash tablahash_armar_diccionario(char *nombreArchivo) {
  FILE *archivo = fopen(nombreArchivo, "rb");
  if (archivo == NULL)
    return NULL;
  TablaHash tabla = tablahash_crear(1000, copiar_palabra, comparar_palabras,
                                    destruir_palabras_en_lista,
                                    hashear_palabra);

  while (!feof(archivo)) {
    char palabraAux[30];
    fscanf(archivo, "%s ", palabraAux);
    
    for (int i = 0; palabraAux[i] != '\0'; i++) 
      palabraAux[i] = toupper(palabraAux[i]);

    if (requiere_redimensionar(tabla))
      tablahash_redimensionar(tabla);
    
    tablahash_insertar(tabla, palabraAux);
  }

  fclose(archivo);

  return tabla;
}

/* Retorna 1 si la palabra está en la tabla, 0 si no */
int palabra_correcta(TablaHash tabla, char *palabra) {
  if (tablahash_buscar(tabla, palabra) == -1)
    return 0;
  return 1;
}
