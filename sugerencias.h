#include "slist.h"
#include "hash.h"

#ifndef __SUGERENCIAS_
#define __SUGERENCIAS_

typedef struct _SugValidas {
  char *palabraOriginal;
  char **palabrasValidas;
  int cantidad;
  int linea;
} *SugValidas;

SugValidas sug_validas_crear (int linea, char *palabra, int len);

void sug_validas_agregar (SugValidas sugerencias, char *palabra, int len);

int sug_validas_buscar (SugValidas sugerencias, char *palabra);

void sug_validas_destruir (SugValidas sug);

void sug_validas_mostrar(SugValidas sug);

SugValidas buscar_sugerencias (TablaHash tabla, TablaHash erroneas, char *palabra, int linea, int len);

#endif // __SUGERENCIAS_