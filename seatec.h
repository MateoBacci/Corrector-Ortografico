#include "hash.h"
#include "sugerencias.h"
#ifndef __SEA_TEC_
#define __SEA_TEC_

// SEARCH TECHNIQUES

void agregar_palabra_segun_validez (TablaHash diccionario,
                                    TablaHash palabrasErroneas,
                                    SugValidas palabrasAceptadas,
                                    char *palabra);


/**
 * Intercambia cada par de caracteres adyacentes en la palabra.
 * Retorna n-1 palabras nuevas.
 */
void intercambiar_adyacentes (TablaHash diccionario,
                              TablaHash palabrasErroneas,
                              SugValidas aceptadas,
                              char *palabra,
                              int len);

/**
 * Elimina cada caracter de la palabra.
 * Retorna n palabras nuevas.
 */
void eliminar_caracteres (TablaHash diccionario,
                          TablaHash palabrasErroneas,
                          SugValidas aceptadas,
                          char *palabra,
                          int len);

/**
 * Reemplaza cada caracter de la palabra por cada caracter del alfabeto. 
 * Retorna 26n palabras nuevas
 */
void reemplazar_caracteres (TablaHash diccionario,
                            TablaHash palabrasErroneas,
                            SugValidas aceptadas,
                            char *palabra,
                            int len);


/**
 * Inserta cada caracter del alfabeto en cualquier posición de la palabra.
 * Retorna 26(n-1) palabras nuevas.
 */
void insertar_caracter (TablaHash diccionario,
                        TablaHash palabrasErroneas,
                        SugValidas aceptadas,
                        char *palabra,
                        int len);


/**
 * Separa la palabra entre cada par de letras adyacentes generando dos palabras
 * nuevas. Retorna 2n-2 palabras nuevas.  
 */
void separar_palabra (TablaHash diccionario,
                     TablaHash palabrasErroneas,
                     SugValidas aceptadas,
                     char *palabra,
                     int len);

#endif // __SEA_TEC_