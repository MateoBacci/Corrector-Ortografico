#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "sugerencias.h"
#include "texman.h"
#include "hash.h"


int main() {
  char *nombreArchivo = "lemario.txt";
  
  TablaHash tabla = tablahash_armar(nombreArchivo);
  printf("CANT ELEMENTOS: %d CAPACIDAD TOTAL: %d FACTOR CARGA: %.2f%c\n",
          tabla->numElems,   tabla->capacidad,   tabla->factorCarga * 100, 37); 
  
  FILE *archivoConTexto = fopen("prueba.txt", "rb");
  assert(archivoConTexto != NULL);

  while (!feof(archivoConTexto)) {
    char palabra[30];
    fscanf(archivoConTexto, "%s ", palabra);
    normalize_word(palabra);
    if (!palabra_correcta(tabla, palabra)) {
      //buscar_sugerencias(tabla, palabra);
    }
  } 

  fclose(archivoConTexto);
  tablahash_destruir(tabla);
  return 0;
}



/*
int main (int argc, char** argv) {
  if (argc != 3) return 1;
  char *inputFileName = argv[1];
  char *outputFileName = argv[2];l
*/