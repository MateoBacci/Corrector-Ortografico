#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#include "seatec.h"
#include "funciones_tabla.h"
#include "hash.h"
#include "sugerencias.h"

char *readfile(FILE *archivo, int *linea) {
  char buf[40];
  int len = 0, bandera = 0;
  while (!bandera) {
    char c = fgetc(archivo);
    if (isalpha(c)) {
      buf[len] = toupper(c);
      len++;
    }
    else {
      if (c == '\n')
        *linea += 1;
      bandera = 1;
    }
  }
  if (len > 0) {
    char *salida = strndup(buf, len);
    /* char *salida = malloc(sizeof(char) * len + 1);
    buf[len] = '\0';
    memcpy(salida, buf, len); */
    return salida;
  }
  return NULL;
}



int main() {
  TablaHash tabla = tablahash_armar_diccionario("lemario.txt");
  
  FILE *archivo_entrada = fopen("lorem.html", "rb");
  SList listaSug = lista_crear();
  int linea = 1;
  while (!feof(archivo_entrada)) { 
    char *palabra = readfile(archivo_entrada, &linea);

    if (palabra == NULL)
      continue;

    int len = strlen(palabra);
    if (tablahash_buscar(tabla, palabra) == -1) {
      printf("Buscando sugerencias para \"%s\"...\n", palabra);
      SugValidas sugNueva = corregir_palabra(tabla, palabra, linea, len);
      listaSug = lista_agregar (listaSug, sugNueva,
          (FuncionCopiaLista)sug_validas_copiar, (FuncionComparaLista)sug_validas_comparar, 0);
      //sug_validas_mostrar(sugNueva);
      sug_validas_destruir(sugNueva);
    } 
    free(palabra);
  }
  fclose(archivo_entrada);
  imprimir_archivo_salida("salida_nueva.txt", listaSug);
  lista_destruir(listaSug, (FuncionDestruyeLista)sug_validas_destruir);
  tablahash_destruir(tabla);
  
  
  return 0;
}