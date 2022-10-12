#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "newio.h"
#include "slist.h"
#include "sugerencias.h"
#include "funciones_tabla.h"
#include "hash.h"
#include "seatec.h"


/**
 * Crea una nueva estructura SugValidas.
 */
SugValidas sug_validas_crear(int linea, char *palabra, int len) {
  SugValidas sug = malloc(sizeof(struct _SugValidas));
  sug->cantidad = 0;
  sug->linea = linea;
  sug->palabraOriginal = stringndup(palabra, len);
  sug->palabrasValidas = malloc(sizeof(char *) * 5);
  return sug;
}

/**
 * Agrega una palabra a las sugerencias de una palabra errónea. 
 */
void sug_validas_agregar(SugValidas sugerencias, char *palabra) {
  if (sug_validas_buscar(sugerencias, palabra) == 0) {  //Si no está, la agrega.
    int pos = sugerencias->cantidad;
    sugerencias->palabrasValidas[pos] = stringdup(palabra);
    sugerencias->cantidad++;
  }
}

/**
 * Busca si una palabra se encuentra en las sugerencias de una palabra errónea.  
 */
int sug_validas_buscar(SugValidas sugerencias, char *palabra) {
  for (int i = 0; i < sugerencias->cantidad; i++) {
    if (strcmp(sugerencias->palabrasValidas[i], palabra) == 0)
      return 1;
  }
  return 0;
}

/**
 * Retorna una copia de la estructura.
 */
SugValidas sug_validas_copiar(SugValidas sugOriginal) {
  SugValidas sugNueva = malloc(sizeof(struct _SugValidas));
  sugNueva->cantidad = sugOriginal->cantidad;
  sugNueva->linea = sugOriginal->linea;
  sugNueva->palabraOriginal = stringdup(sugOriginal->palabraOriginal);
  sugNueva->palabrasValidas = malloc(sizeof(char *) * 5);
  for (int i = 0; i < sugOriginal->cantidad; i++) {
    sugNueva->palabrasValidas[i] = stringdup(sugOriginal->palabrasValidas[i]);
  }
  return sugNueva;
}

/**
 * Destruye la estructura. 
 */
void sug_validas_destruir(SugValidas sug) {
  for (int i = 0; i < sug->cantidad; i++) {
    free(sug->palabrasValidas[i]);
  }
  free(sug->palabrasValidas);
  free(sug->palabraOriginal);
  free(sug);
}

/**
 * Copia las palabras de una tabla hash en un arreglo de strings para 
 * recorrerlo en las distancias 2 y 3 del programa.
 */
char **de_listas_a_strings(SList *listas, unsigned cantListas,
                           unsigned cantStrings) {
  char **arregloNuevo = malloc(sizeof(char *) * cantStrings);
  unsigned j = 0;
  for (unsigned i = 0; i < cantListas; i++) {
    for (SList aux = listas[i]; aux != NULL; aux = aux->sig) {
      arregloNuevo[j] = stringdup((char *) aux->dato);
      j++;
    }
  }
  return arregloNuevo;
}

/**
 * Destruye un arreglo de strings. 
 */
void arreglo_destruir(char **arreglo, unsigned total) {
  for (unsigned i = 0; i < total; i++) {
    free(arreglo[i]);
  }
  free(arreglo);
}

/**
 * Dada una palabra errónea, busca hasta 5 sugerencias que se encuentren en el
 * diccionario aplicando como mucho 3 técnicas.
 */
SugValidas corregir_palabra(TablaHash diccionario, char *palabra, int linea,
                            unsigned len) {

  SugValidas sugerencias = sug_validas_crear(linea, palabra, len);
  TablaHash tablaErroneas = tablahash_crear(5 * len, copiar_palabra,
                                            comparar_palabras,
                                            destruir_palabras_en_lista,
                                            hashear_palabra);

  for (int distancia = 0; distancia < 3 && sugerencias->cantidad < 5;
       distancia++) {
    printf("Distancia: %d, FdC: %f\n", distancia,
           tablaErroneas->factorCarga * 100);
    if (distancia == 0) {
      buscar_sugerencias_por_palabra(diccionario, tablaErroneas, sugerencias,
                                     palabra, len, distancia);
    } else if (distancia == 1) {
      unsigned total = tablaErroneas->numElems;
      char **arrayErroneas = de_listas_a_strings(tablaErroneas->palabras,
                                                 tablaErroneas->capacidad,
                                                 tablaErroneas->numElems);

      for (unsigned i = 0; i < total && sugerencias->cantidad < 5; i++) {
        unsigned actualLen = strlen(arrayErroneas[i]);
        buscar_sugerencias_por_palabra(diccionario, tablaErroneas, sugerencias,
                                       arrayErroneas[i], actualLen, distancia);
      }
      arreglo_destruir(arrayErroneas, total);

    } else {                    // distancia == 2
      for (unsigned i = 0;
           i < tablaErroneas->capacidad && sugerencias->cantidad < 5; i++) {
        if (tablaErroneas->palabras[i] == NULL)
          continue;
        for (SList aux = tablaErroneas->palabras[i]; aux != NULL;
             aux = aux->sig) {
          char *dato = (char *) aux->dato;
          unsigned int actualLen = strlen(dato);
          buscar_sugerencias_por_palabra(diccionario, tablaErroneas,
                                         sugerencias, dato, actualLen,
                                         distancia);
        }
      }
    }
  }
  tablahash_destruir(tablaErroneas);
  return sugerencias;
}


/**
 * Dada una palabra, busca sugerencias hasta encontrar 5 distintas o hasta 
 * utilizar las 5 técnicas dadas.
 */
void buscar_sugerencias_por_palabra(TablaHash tabla, TablaHash erroneas,
                                    SugValidas sugerencias, char *palabra,
                                    unsigned len, int paso) {
  char *copia = malloc(sizeof(char) * (len + 2));
  for (int opcion = 0; sugerencias->cantidad < 5 && opcion < 5; opcion++) {
    switch (opcion) {
    case 0:
      intercambiar_adyacentes(tabla, erroneas, sugerencias, palabra, copia, len,
                              paso);
      break;
    case 1:
      eliminar_caracteres(tabla, erroneas, sugerencias, palabra, copia, len,
                          paso);
      break;
    case 2:
      reemplazar_caracteres(tabla, erroneas, sugerencias, palabra, copia, len,
                            paso);
      break;
    case 3:
      insertar_caracter(tabla, erroneas, sugerencias, palabra, copia, len,
                        paso);
      break;
    case 4:
      separar_palabra(tabla, erroneas, sugerencias, palabra, copia, len);
      break;
    }
  }
  free(copia);
}
