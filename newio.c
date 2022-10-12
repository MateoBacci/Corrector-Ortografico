#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "slist.h"
#include "sugerencias.h"

/* Toma un string y su largo y retorna una copia, pidiendo memoria. */
char *stringndup(char *string, unsigned len) {
  char *newString = malloc(sizeof(char) * len + 1);
  memcpy(newString, string, len);
  newString[len] = '\0';
  return newString;
}

/* Toma un string y retorna una copia, calculando su largo y pidiendo memoria. */
char *stringdup(char *string) {
  unsigned len = strlen(string);
  char *newString = malloc(sizeof(char) * len + 1);
  memcpy(newString, string, len);
  newString[len] = '\0';
  return newString;
}

/**
 * Toma un archivo como argumento y retorna una palabra únicamente con letras
 * mayúsculas. Además actualiza la linea que está leyendo del archivo en caso
 * de ser necesario. Retorna NULL si no leyó una palabra.
 */
char *readfile(FILE * archivo, int *linea) {
  char buf[40];
  int len = 0, bandera = 0;
  while (!bandera) {
    char c = fgetc(archivo);
    // Si es una letra, en caso de ser necesario, la hace mayúscula y la guarda.
    if (isalpha(c)) {
      buf[len] = toupper(c);
      len++;
    }
    // Si no, no la guarda y termina el bucle.
    else {
      if (c == '\n')
        *linea += 1;
      bandera = 1;
      buf[len] = '\0';
    }
  }
  if (len > 0) {
    char *salida = stringndup(buf, len);
    return salida;
  }
  return NULL;
}

/**
 * Escribe el archivo de salida con las sugerencias obtenidas para cada palabra
 * errónea del archivo de entrada.
 */
void writefile(char *nombreArchivo, SList listaSugerencias) {
  FILE *archivo = fopen(nombreArchivo, "wb");
  SList aux = listaSugerencias;
  for (; aux != NULL; aux = aux->sig) {
    SugValidas dato = (SugValidas) aux->dato;
    fprintf(archivo, "Linea %d, ", dato->linea);
    fprintf(archivo, "\"%s\" no esta en el diccionario.\n",
            dato->palabraOriginal);

    if (dato->cantidad == 0) {
      fprintf(archivo, "No se encontraron sugerencias para esa palabra.\n");
      continue;
    }
    fprintf(archivo, "Quizas quiso decir: ");
    for (int i = 0; i < dato->cantidad - 1; i++)
      fprintf(archivo, "%s, ", dato->palabrasValidas[i]);
    fprintf(archivo, "%s.\n", dato->palabrasValidas[dato->cantidad - 1]);
  }
  fclose(archivo);
}
