#include <stdio.h>
#include "slist.h"

#ifndef __NEW_IO__
#define __NEW_IO__

char *stringndup (char *string, unsigned len);

char *stringdup (char *string);

char *readfile (FILE *archivo, int *linea);

void writefile (char *nombreArchivo, SList listaSugerencias);

#endif