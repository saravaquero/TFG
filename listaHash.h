#ifndef LISTAHASH_H
#define LISTAHASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Lista_H ListaH;
typedef struct Nodo_H NodoH;

struct Nodo_H{
    char* clave;                    // Ip del clave
    char* destino;
    int sport;
    int dport;
    double tiempo;                  // Ultimo tiempo de llegada
    double jitter;
    double delta;
    double ancho_banda;
    double perdidas;
    double mos;
    struct Nodo_H *siguiente;
};

struct Lista_H{
    NodoH * nodo;
};
/********************* Declaracion de funciones públicas **********************/

NodoH *NodoH_ini();

void NodoH_free(NodoH *nodo);
/* Inicializa la listaa */
ListaH *crea_ListaH();

/* Libera la listaa */
void borra_ListaH(ListaH *lista);

/* Inserta al principio de la listaa realizando una copia de los elemento. */
ListaH *InsertaPrimero_ListaH(ListaH *lista, const char *clave, const char *destino,const int puerto_origen, const int puerto_destino, const double tiempo, const double jitter, const double delta,  const double ancho_banda, const double perdidas, const double mos);
/* Inserta al final de la listaa realizando una copia de los elemento. */
ListaH *InsertaFinal_ListaH(ListaH *lista, const char *clave, const char *destino,const int puerto_origen, const int puerto_destino, const double tiempo,  const double jitter, const double delta, const double ancho_banda,  const double perdidas, const double mos);
/* Extrae del principio de la listaa realizando una copia del clave. */
char* ExtraePrimero_ListaH(ListaH *lista);

/* Extrae del final de la listaa realizando una copia del clave. */
char* ExtraeFinal_ListaH(ListaH *lista);

/* Comprueba si la listaa está vacía o no. */
int esVacia_ListaH(const ListaH *lista);

/* Devuelve el tamaño de la listaa. */
int tam_ListaH(const ListaH *lista);

/* Imprime una listaa devolviendo el tamanio. */
int imprime_ListaH(const ListaH *lista);

/* Comprueba si el clave esta en la listaa. */
int existeElemento_ListaH(const ListaH *lista, char* ipconst , char *destino, int puerto_origen, int puerto_destino);

/* Eliminamos el NodoH que contiene el clave de la listaa*/
int extraeElemento_ListaH(ListaH *lista, char* ip);

/* Devolvemos la ip del clave del NodoH*/
char* getHclave(NodoH *nodo);

/* Devolvemos eltiempo de llegada guardado en el NodoH*/
double getHTiempo(NodoH *nodo);

/* Modificamos eltiempo de llegada guardado en el NodoH*/
int setHTiempo(NodoH *nodo, double tmp);



/* Devolvemos el NodoH vecino */
NodoH *getHNext(NodoH *nodo);

/* Devulve el NodoH que tenga la ip */
NodoH *getHNodo(ListaH *lista, char* clave, char* destino, int puerto_origen,int puerto_destino);

/* Devuelve un NodoH dado una posicion */
NodoH *getHNodePos(ListaH *lista, int pos);
double getHDelta(NodoH*nodo) ;
double getHJitter(NodoH*nodo) ;
char* getHDestino(NodoH*nodo) ;
int getH_sport(NodoH*nodo);
int getH_dport(NodoH*nodo);
double getHAnchoBanda(NodoH*nodo) ;
double getHPerdidas(NodoH*nodo) ;
double getHMOS(NodoH*nodo) ;

/* Se realiza una copia de la listaa */
//ListaH *ListaH_copy(const ListaH *lista);

#endif /*ListaH_H*/
