#ifndef __HASH_
#define __HASH_
#include "slist.h"

/** Retorna una copia fisica del dato */
typedef char *(*FuncionCopiaTabla)(char *dato);

/** Retorna un entero negativo si dato1 < dato2, 0 si son iguales y un entero
 * positivo si dato1 > dato2  */
typedef int (*FuncionComparadoraTabla)(char *dato1, char *dato2);

/** Libera la memoria alocada para el dato */ 
typedef void (*FuncionDestructoraTabla)(SList lista);

typedef unsigned (*FuncionHash)(char *dato, unsigned total);


typedef struct _TablaHash {
  SList *palabras;
  unsigned capacidad;
  unsigned numElems;
  float factorCarga;
  FuncionCopiaTabla copia;
  FuncionComparadoraTabla comp;
  FuncionDestructoraTabla destr;
  FuncionHash hash;
} *TablaHash;


unsigned hash_index (char *word, unsigned total);


/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned capacidad, FuncionCopiaTabla copia,
                          FuncionComparadoraTabla comp, FuncionDestructoraTabla destr,
                          FuncionHash hash);


/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash tabla);

/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 */
void tablahash_insertar(TablaHash tabla, char *dato, int esDict);

/**
 * Retorna la posición del dato si está en la tabla, o -1 si no lo encuentra
 */
int tablahash_buscar(TablaHash tabla, char *dato);

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, char *dato);

int requiere_redimensionar (TablaHash tabla);

void tablahash_redimensionar (TablaHash tabla, unsigned multiplo, int esDict);

TablaHash tablahash_armar_diccionario (char *nombreArchivo);

int palabra_correcta (TablaHash tabla, char *palabra);

#endif // __HASH_