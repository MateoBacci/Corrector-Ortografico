all: texman slist funciones hash seatec sugerencias main prueba

texman: texman.c texman.h
	gcc -c texman.c 

slist: slist.c slist.h
	gcc -c slist.c

funciones: funciones_tabla.c funciones_tabla.h hash.h slist.h
	gcc -c funciones_tabla.c

hash: hash.c hash.h texman.h slist.h funciones_tabla.h
	gcc -c hash.c 

seatec: seatec.c seatec.h slist.o hash.h slist.h
	gcc -c seatec.c 

sugerencias: sugerencias.c sugerencias.h funciones_tabla.h hash.h
	gcc -c sugerencias.c

main: main.c sugerencias.h texman.h hash.h 
	gcc -Wall -g -o main main.c texman.o hash.o seatec.o funciones_tabla.o slist.o sugerencias.o

prueba: prueba.c seatec.h hash.h funciones_tabla.h
	gcc -Wall -g -o prueba prueba.c texman.o hash.o funciones_tabla.o slist.o sugerencias.o seatec.o

clean: 
	rm *.o
	rm main