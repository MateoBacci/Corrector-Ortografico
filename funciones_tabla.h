#include "slist.h"
#ifndef __FUNCIONES_TABLA__
#define __FUNCIONES_TABLA__


char *copiar_palabra (char *dato);

int comparar_palabras (char *dato1, char *dato2);

void destruir_palabras_en_lista (SList lista);

unsigned hashear_palabra (char *dato, unsigned total);


#endif // __FUNCIONES_TABLA__
