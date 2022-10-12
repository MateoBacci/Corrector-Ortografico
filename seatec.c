#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "newio.h"
#include "sugerencias.h"
#include "hash.h"
#include "slist.h"

// SEARCH TECHNIQUES
#define CANT_LETRAS 26

/**
 * Dada una sugerencia generada con alguna de las técnicas, verifica si está en
 * el diccionario, y en base a eso guarda una copia en palabrasAceptadas o en  
 * palabrasErroneas.
 */
void agregar_palabra_segun_validez(TablaHash diccionario,
                                   TablaHash palabrasErroneas,
                                   SugValidas palabrasAceptadas,
                                   char *palabra, int paso) {
  if (palabra_correcta(diccionario, palabra)) {
    sug_validas_agregar(palabrasAceptadas, palabra);
  } else if (paso < 2) {
    if (requiere_redimensionar(palabrasErroneas))
      tablahash_redimensionar(palabrasErroneas);
    tablahash_insertar(palabrasErroneas, palabra);
  }
}

/**
 * Dada una palabra, genera nuevas intercambiando caracteres adyacentes. 
 */
void intercambiar_adyacentes(TablaHash diccionario, TablaHash palabrasErroneas,
                             SugValidas palabrasAceptadas, char *palabra,
                             char *copia, int len, int paso) {
  if (len == 1)
    return;

  for (int i = 0; i < len - 1 && palabrasAceptadas->cantidad < 5; i++) {
    memcpy(copia, palabra, len);
    copia[len] = '\0';
    if (copia[i] != copia[i + 1]) {
      char charAux = copia[i];
      copia[i] = copia[i + 1];
      copia[i + 1] = charAux;
      agregar_palabra_segun_validez(diccionario, palabrasErroneas,
                                    palabrasAceptadas, copia, paso);
    }
  }
}

/**
 * Dada una palabra, genera nuevas eliminando de a un caracter. 
 */
void eliminar_caracteres(TablaHash diccionario, TablaHash palabrasErroneas,
                         SugValidas palabrasAceptadas, char *palabra,
                         char *copia, int len, int paso) {
  if (len == 1)
    return;
  for (int i = 0; i < len && palabrasAceptadas->cantidad < 5; i++) {
    memcpy(copia, palabra, len);
    memmove(copia + i, copia + i + 1, len - i - 1);
    copia[len - 1] = '\0';
    if (palabra[i] != palabra[i + 1]) {
      agregar_palabra_segun_validez(diccionario, palabrasErroneas,
                                    palabrasAceptadas, copia, paso);
    }
  }
}

/**
 * Dada una palabra, genera nuevas reemplazando un caracter por uno distinto. 
 */
void reemplazar_caracteres(TablaHash diccionario, TablaHash palabrasErroneas,
                           SugValidas palabrasAceptadas, char *palabra,
                           char *copia, int len, int paso) {
  for (int i = 0; i < len; i++) {
    for (int letra = 'A';
         letra < CANT_LETRAS + 'A' && palabrasAceptadas->cantidad < 5;
         letra++) {
      memcpy(copia, palabra, len);
      copia[len] = '\0';
      if (copia[i] != letra) {
        copia[i] = letra;
        agregar_palabra_segun_validez(diccionario, palabrasErroneas,
                                      palabrasAceptadas, copia, paso);
      }
    }
  }
}

/**
 * Inserta una letra entre dos caracteres de una palabra. 
 */
char *insertar_nueva_letra(char *palabra, char letra, int pos, int len) {
  int flag = 0;
  char *palabraNueva = malloc(len + 1);
  for (int i = 0; i < len; i++) {
    if (i != pos)
      palabraNueva[i] = palabra[i - flag];
    else {
      palabraNueva[i] = letra;
      flag = 1;
    }
  }
  palabraNueva[len] = '\0';
  return palabraNueva;
}


/**
 * Dada una palabra, genera nuevas insertando un caracter entre dos de la 
 * palabra original. 
 */
void insertar_caracter(TablaHash diccionario, TablaHash palabrasErroneas,
                       SugValidas palabrasAceptadas, char *palabra, char *copia,
                       int len, int paso) {
  for (int i = 0; i < len + 1; i++) {
    for (char letra = 'A';
         letra < CANT_LETRAS + 'A' && palabrasAceptadas->cantidad < 5;
         letra++) {
      memcpy(copia, palabra, len);
      copia[len + 1] = '\0';
      if (palabra[i] != letra) {
        memcpy(copia + i + 1, copia + i, len - i);
        copia[i] = letra;
        agregar_palabra_segun_validez(diccionario, palabrasErroneas,
                                      palabrasAceptadas, copia, paso);
      }
    }
  }
}

/**
 * Dada una palabra, la separa en dos distintas para verificar si cada parte
 * está en el diccionario. 
 */
void separar_palabra(TablaHash diccionario, SugValidas palabrasAceptadas,
                     char *palabra, char *copia, int len) {
  for (int i = 1; i < len && palabrasAceptadas->cantidad < 5; i++) {
    char *substr1 = stringndup(palabra, i);
    char *substr2 = stringndup(palabra + i, len - i);
    copia[i + 1] = '\0';
    if (palabra_correcta(diccionario, substr1)
        && palabra_correcta(diccionario, substr2)) {
      memcpy(copia, substr1, i);
      memcpy(copia + i, " ", 1);
      memcpy(copia + i + i, substr2, len - i);
      copia[len + 1] = '\0';
      sug_validas_agregar(palabrasAceptadas, copia);
    }
    free(substr1);
    free(substr2);
  }
}
