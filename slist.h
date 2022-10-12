#ifndef __S_LIST_
#define __S_LIST_

typedef void (*FuncionDestruyeLista)(void *dato);
typedef void *(*FuncionCopiaLista)(void *dato);
typedef int (*FuncionComparaLista)(void *dato1, void *dato2);

typedef struct _SNodo {
  void *dato;
  struct _SNodo *sig;
} SNodo;

typedef SNodo *SList;

SList lista_crear();

void lista_destruir(SList lista, FuncionDestruyeLista dest);

SList lista_agregar(SList lista, void *dato, FuncionCopiaLista copiar);

void *lista_buscar(SList lista, void *dato, FuncionComparaLista comp);

#endif // __S_LIST_