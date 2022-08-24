#ifndef __S_LIST_
#define __S_LIST_

typedef struct _SNodo {
  char *dato;
  struct _SNodo *sig;
} SNodo;

typedef SNodo *SList;

SList lista_crear();

void lista_destruir(SList lista);

SList lista_agregar(SList lista, char *dato);

char *lista_buscar (SList lista, char *dato);

#endif // __S_LIST_