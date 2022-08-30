#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sugerencias.h"
#include "hash.h"
#include "slist.h"

/*
n = largo de la palabra.

Palabras nuevas obtenidas por cada técnica:
  - Intercambiar: n-1.
  - Eliminar: n.
  - Reemplazar: 26n.
  - Insertar: 26(n+1).
  - Separar: n-1.

Total: 54n + 25 + (n - 1 pares de palabras generados por Separar).

*/

#define CANT_DE_LETRAS 26


void agregar_palabra_segun_validez (TablaHash diccionario,
                                    TablaHash palabrasErroneas,
                                    SugValidas palabrasAceptadas,
                                    char *palabra, int len, int paso) {
    
  if (palabra_correcta(diccionario, palabra)) {
    sug_validas_agregar(palabrasAceptadas, palabra, len);
  }
  else if (paso < 2){  
    /* if (requiere_redimensionar(palabrasErroneas)) {
      tablahash_redimensionar(palabrasErroneas, 5, 0);
    } */
    tablahash_insertar(palabrasErroneas, palabra, 0);
  }
}


void chequear_ambas_palabras (SugValidas validas, TablaHash tabla,
                              char *palabra, int len, int pos) {
  
  char *substr1 = strndup(palabra, pos);
  char *substr2 = strndup(palabra + pos + 1, len);
  if (palabra_correcta(tabla, substr1) && palabra_correcta(tabla, substr2)) 
    sug_validas_agregar(validas, palabra, len);

  free(substr1);
  free(substr2);
}

void intercambiar_adyacentes (TablaHash diccionario, TablaHash palabrasErroneas,
                              SugValidas palabrasAceptadas, char *palabra,
                              int len, int paso) {
  
  for (int i = 0; i < len - 1 && palabrasAceptadas->cantidad < 5; i++) {
    if (palabra[i] != palabra[i+1]) {
      char *palabraNueva = strndup(palabra, len);
      char charAux = palabra[i];
      palabraNueva[i] = palabra[i+1];
      palabraNueva[i+1] = charAux;
      agregar_palabra_segun_validez(diccionario, palabrasErroneas, palabrasAceptadas,
                                    palabraNueva, len, paso);
      free(palabraNueva);
    }
  }
}

void eliminar_caracteres (TablaHash diccionario, TablaHash palabrasErroneas,
                           SugValidas palabrasAceptadas, char *palabra, int len, int paso) {
  
  int cantidadAceptadas = 0;
  for (int i = 0; i < len && palabrasAceptadas->cantidad < 5; i++) {
    char *palabraNueva = malloc(sizeof(char) * len);
    palabraNueva[0] = '\0';
    strncat(palabraNueva, palabra, i);
    strncat(palabraNueva, palabra + i + 1, len);
    agregar_palabra_segun_validez (diccionario, palabrasErroneas, palabrasAceptadas,
                                  palabraNueva, len - 1, paso);
    free(palabraNueva);
  }
}

char *reemplazar_una_letra (char *palabra, int pos, char letra, int len) {
  char *palabraNueva = strndup(palabra, len);
  palabraNueva[pos] = letra; 
  return palabraNueva;
}

void reemplazar_caracteres (TablaHash diccionario, TablaHash palabrasErroneas,
                             SugValidas palabrasAceptadas, char *palabra, int len, int paso) {
  int cantidadAceptadas = 0;

  for (int i = 0; i < len; i++) {
    for (int letra = 'A'; letra < CANT_DE_LETRAS + 'A' && palabrasAceptadas->cantidad < 5; letra++){
      if (palabra[i] != letra) {
        char *palabraNueva = reemplazar_una_letra(palabra, i, letra, len);

        agregar_palabra_segun_validez(diccionario, palabrasErroneas,
                                      palabrasAceptadas, palabraNueva, len, paso);

        free(palabraNueva);
      }
    }
  }
}
 
char *insertar_nueva_letra (char *palabra, char letra, int pos, int len) {
  int flag = 0;
  char *palabraNueva = malloc(len + 1);
  for (int i = 0; i < len ; i++) {
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

void insertar_caracter (TablaHash diccionario, TablaHash palabrasErroneas,
                         SugValidas palabrasAceptadas, char *palabra, int len, int paso) {
  
  int cantidadAceptadas = 0; 
  for (int i = 0; i < len + 1; i++) {
    for (char letra = 'A'; letra < CANT_DE_LETRAS + 'A' && palabrasAceptadas->cantidad < 5; letra++) {
      char *palabraNueva = insertar_nueva_letra(palabra, letra, i, len+1);

      agregar_palabra_segun_validez(diccionario, palabrasErroneas,
                                      palabrasAceptadas, palabraNueva, len + 1, paso);

      free(palabraNueva);
    }
  }
}

void separar_palabra (TablaHash diccionario, TablaHash palabrasErroneas,
                      SugValidas palabrasAceptadas, char *palabra, int len) {
  for (int i = 1; i < len && palabrasAceptadas->cantidad < 5; i++) {
    char *palabraNueva = malloc(sizeof(char) * len + 2);
    palabraNueva[0] = '\0';
    strncat(palabraNueva, palabra, i);
    strcat(palabraNueva, " ");
    strncat(palabraNueva, palabra + i, len);
    chequear_ambas_palabras(palabrasAceptadas, diccionario, palabraNueva, len + 1, i);
    free(palabraNueva);
  }
} 

