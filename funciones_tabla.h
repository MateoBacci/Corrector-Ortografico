#include "slist.h"
#ifndef __FUNCIONES_TABLA_
#define __FUNCIONES_TABLA_


char *copiar_palabra (char *dato);

int comparar_palabras (char *dato1, char *dato2);

void destruir_palabra (SList lista);

unsigned hashear_palabra (char *dato, unsigned total);


#endif // __FUNCIONES_TABLA_
