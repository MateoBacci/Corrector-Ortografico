all: slist funciones hash seatec sugerencias main prueba

slist: slist.c slist.h
	gcc -c slist.c

funciones: funciones_tabla.c funciones_tabla.h hash.h slist.h
	gcc -c funciones_tabla.c 

hash: hash.c hash.h slist.h funciones_tabla.h
	gcc -c hash.c 

seatec: seatec.c seatec.h slist.o hash.h slist.h
	gcc -c seatec.c

sugerencias: sugerencias.c sugerencias.h funciones_tabla.h hash.h
	gcc -c sugerencias.c

#main: main.c sugerencias.h hash.h 
#	gcc -Wall -Wextra -Werror -std=c99 -g -o main main.c hash.o seatec.o funciones_tabla.o slist.o sugerencias.o

prueba: prueba.c seatec.h hash.h funciones_tabla.h
	gcc -Wall -Wextra -Werror -g -o prueba prueba.c hash.o funciones_tabla.o slist.o sugerencias.o seatec.o

clean: 
	rm *.o
	rm main