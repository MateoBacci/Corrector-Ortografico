#include <stdio.h>
#include <string.h>
#include "seatec.h"
#include "funciones_tabla.h"
#include "hash.h"
#include "texman.h"
#include "sugerencias.h"

  
int main() {
  TablaHash tabla = tablahash_armar("lemario.txt");
  TablaHash tablaErroneas = tablahash_crear(244, copiar_palabra, comparar_palabras, 
                                    destruir_palabra, hashear_palabra);
  
  FILE *archivo = fopen("lorem.html", "rb");
  while (!feof(archivo)){ 
    char palabra[30];
    fscanf(archivo, "%s ", palabra);
    normalize_word(palabra);
    int len = strlen(palabra);
    SugValidas aceptadas = buscar_sugerencias(tabla, tablaErroneas, palabra, 0, len);
    sug_validas_mostrar(aceptadas);
    sug_validas_destruir(aceptadas);
  }
  tablahash_destruir(tabla);
  tablahash_destruir(tablaErroneas);
  
  fclose(archivo);
  
  return 0;
}