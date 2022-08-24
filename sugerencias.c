#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sugerencias.h"
#include "funciones_tabla.h"
#include "hash.h"
#include "seatec.h"

SugValidas sug_validas_crear (int linea, char *palabra, int len) {
  SugValidas sug = malloc(sizeof(struct _SugValidas));
  sug->cantidad = 0;
  sug->linea = linea;
  sug->palabraOriginal = malloc(sizeof(char) * len + 1);
  sug->palabraOriginal[len] = '\0';
  memcpy(sug->palabraOriginal, palabra, len);
  sug->palabrasValidas = malloc(sizeof(char *) * 5);
  return sug;
}

void sug_validas_agregar (SugValidas sugerencias, char *palabra, int len) {
  int pos = sugerencias->cantidad;
  sugerencias->palabrasValidas[pos] = malloc(sizeof(char) * len + 1);
  sugerencias->palabrasValidas[pos][len] = '\0';
  memcpy(sugerencias->palabrasValidas[pos], palabra, len);
  sugerencias->cantidad++;
}

int sug_validas_buscar (SugValidas sugerencias, char *palabra) {
  for (int i = 0; i < sugerencias->cantidad; i++) {
    if (strcmp(sugerencias->palabrasValidas[i], palabra) == 0) 
      return 1;
  }
  return 0;
}

void sug_validas_destruir (SugValidas sug) {
  for (int i = 0; i < sug->cantidad; i++) {
    free(sug->palabrasValidas[i]);
  }
  free(sug->palabrasValidas);
  free(sug->palabraOriginal);
  free(sug);
}

void sug_validas_mostrar(SugValidas sug) {
  printf("Palabra original: %s\n", sug->palabraOriginal);
  printf("Cantidad: %d\n", sug->cantidad);
  printf("Sugerencias: ");
  for (int i = 0; i < sug->cantidad; i++) {
    printf("%s - ", sug->palabrasValidas[i]);
  }
  puts("");
}



SugValidas buscar_sugerencias (TablaHash tabla, TablaHash erroneas, char *palabra,
                               int linea, int len) {

  SugValidas palabrasAceptadas = sug_validas_crear(linea, palabra, len);

  int opcion = 0, pasos = 0;
  
  while (palabrasAceptadas->cantidad < 5 && pasos < 3) {
    switch (opcion) {
      case 0:
        intercambiar_adyacentes(tabla, erroneas, palabrasAceptadas, palabra, len); 
        break;
      case 1:
        eliminar_caracteres(tabla, erroneas, palabrasAceptadas, palabra, len);
        break;
      case 2:
        reemplazar_caracteres(tabla, erroneas, palabrasAceptadas, palabra, len);
        break;
      case 3:
        insertar_caracter(tabla, erroneas, palabrasAceptadas, palabra, len);
        break;
      //case 4:
        
    }
    if (opcion < 4)
      opcion++;
    else {
      opcion = 0;
      pasos++;
    }
  }
  return palabrasAceptadas;
 
}