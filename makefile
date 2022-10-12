FLAGS = -Wall -Wextra -g -Werror -std=c99
INDENT = indent -kr -brf -i2 -l80 -nut


all: slist funciones hash seatec sugerencias newio main

indent: slist.c funciones_tabla.c hash.c seatec.c sugerencias.c newio.c main.c
	$(INDENT) slist.c
	$(INDENT) funciones_tabla.c
	$(INDENT) hash.c
	$(INDENT) seatec.c
	$(INDENT) sugerencias.c
	$(INDENT) newio.c
	$(INDENT) main.c 

slist: slist.c slist.h
	gcc $(FLAGS) -c slist.c

funciones: funciones_tabla.c funciones_tabla.h hash.h slist.h
	gcc $(FLAGS) -c funciones_tabla.c 

hash: hash.c hash.h slist.h funciones_tabla.h
	gcc $(FLAGS) -c hash.c 

seatec: seatec.c seatec.h slist.o hash.h slist.h
	gcc $(FLAGS) -c seatec.c

sugerencias: sugerencias.c sugerencias.h funciones_tabla.h hash.h
	gcc $(FLAGS) -c sugerencias.c

newio: newio.c newio.h slist.h sugerencias.h
	gcc $(FLAGS) -c newio.c

main: main.c sugerencias.h hash.h newio.h slist.h 
	gcc $(FLAGS) -o main main.c hash.o seatec.o funciones_tabla.o slist.o sugerencias.o newio.o

clean: 
	rm *.o
	rm main