#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "newio.h"
#include "sugerencias.h"
#include "hash.h"
#include "slist.h"

TablaHash armar_diccionario() {
  char nombreDiccionario[40];
  printf("Nombre del diccionario: ");
  scanf("%s", nombreDiccionario);
  TablaHash diccionario = tablahash_armar_diccionario(nombreDiccionario);
  return diccionario;
}

SList corregir_palabras_erroneas(char *nombreArchivo, TablaHash diccionario) {
  FILE *archivoConTexto = fopen(nombreArchivo, "rb");
  assert(archivoConTexto != NULL);

  SList listaSug = lista_crear();
  int linea = 1;
  while (!feof(archivoConTexto)) {
    char *palabra = readfile(archivoConTexto, &linea);

    if (palabra == NULL)
      continue;

    int len = strlen(palabra);
    if (tablahash_buscar(diccionario, palabra) == -1) {
      printf("Buscando sugerencias para \"%s\"...\n", palabra);
      SugValidas sugNueva = corregir_palabra(diccionario, palabra, linea, len);
      listaSug =
          lista_agregar(listaSug, sugNueva,
                        (FuncionCopiaLista) sug_validas_copiar);
      sug_validas_destruir(sugNueva);
    }
    free(palabra);
  }
  fclose(archivoConTexto);
  return listaSug;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    perror("Revise las entradas\n");
    return 1;
  }
  char *nombreArchivoEntrada = argv[1];
  char *nombreArchivoSalida = argv[2];

  TablaHash diccionario = armar_diccionario();

  if (diccionario == NULL) {
    perror("Error al crear el diccionario.");
    return 1;
  }

  SList correcciones =
      corregir_palabras_erroneas(nombreArchivoEntrada, diccionario);

  writefile(nombreArchivoSalida, correcciones);

  lista_destruir(correcciones, (FuncionDestruyeLista) sug_validas_destruir);
  tablahash_destruir(diccionario);
  return 0;
}
