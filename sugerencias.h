#include "slist.h"
#include "hash.h"

#ifndef __SUGERENCIAS__
#define __SUGERENCIAS__

typedef struct _SugValidas {
  char *palabraOriginal;
  char **palabrasValidas;
  int cantidad;
  int linea;
} *SugValidas;

SugValidas sug_validas_crear (int linea, char *palabra, int len);

void sug_validas_agregar (SugValidas sugerencias, char *palabra);

int sug_validas_buscar (SugValidas sugerencias, char *palabra);

SugValidas sug_validas_copiar (SugValidas sugOriginal);

void sug_validas_destruir (SugValidas sug);


void buscar_sugerencias_por_palabra (TablaHash tabla,   TablaHash erroneas,
                                     SugValidas sugerencias, char *palabra,
                                     unsigned len, int paso);

SugValidas corregir_palabra (TablaHash diccionario, char *palabra, int linea,
                             unsigned len);

void imprimir_archivo_salida (char *nombreArchivo, SList listaSugerencias);

#endif // __SUGERENCIAS__