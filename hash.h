#ifndef HASH
#define HASH


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>


#include "listaHash.h"


#define OK 0			  /* OK */
#define ERROR -1		  /* ERROR */

#define MAXNUM 16	  /* Maximo del string origen */



typedef struct Nodo_Hash NodoHash;

typedef struct Tabla_Hash {
	int tam;    		/* Tamanio de la tabla hash */
	NodoHash** nodos; 	/* Elementos de la tabla */
} TablaHash;

/*****FUNCIONES***/

int borrarNodo(NodoHash* nodo);

int borrarNodoRC(NodoHash* nodo);

TablaHash* crearTablaHash(int tam);

int borrarTablaHash(TablaHash *tabla);

int funcionHash(TablaHash* tabla, char *origen, char *destino, int source_port, int dst_port);

NodoHash* crearNodoHash(char *origen, char *destino, int puerto_origen, int puerto_destino);

int insertarNodoHash(TablaHash *tabla, char *origen,char *destino, void *info, int puerto_origen, int puerto_destino, u_int32_t ssrc,int payload, double tiempo, u_int32_t timestamp, int bytes);

NodoHash* buscarNodoHash(TablaHash *tabla, char *origen, char *destino, int source_port, int dst_port);

char* getOrigen(NodoHash* nodo);

char* getDestino(NodoHash* nodo);

int getNumRecibidos(NodoHash* nodo);

int setNumRecibidos(NodoHash* nodo, int num);

int getNumPerdidos(NodoHash* nodo);

int setNumPerdidos(NodoHash* nodo, int num);

double getRetardo(NodoHash* nodo);

int setRetardo(NodoHash* nodo, double ret);

double getLlegadaAnterior(NodoHash* nodo);

int setLlegadaAnterior(NodoHash* nodo, double ret);

int getNumBytes(NodoHash* nodo);

int setNumBytes(NodoHash* nodo, int num);
NodoHash* getSiguiente(NodoHash *nodo);
void printTablaHash(TablaHash *tabla);
double getTiempoInicio(NodoHash* nodo);
double getTiempoFinal(NodoHash* nodo);
int getPuertoDestino(NodoHash* nodo) ;
int getPuertoOrigen(NodoHash* nodo);
int getPayload(NodoHash* nodo);
int getSSRC(NodoHash* nodo);
void exportarTablaHash(TablaHash *tabla);
int setJitterAnt(NodoHash* nodo, double jitter) ;
double getJitterAnt(NodoHash* nodo);
int setJitter(NodoHash* nodo, double jitter);
double getJitter(NodoHash* nodo) ;
double getMaxJitter(NodoHash* nodo) ;
int setMaxJitter(NodoHash* nodo, double jitter);
int setTimeAnterior(NodoHash* nodo, u_int32_t timestamp);
u_int32_t getTimeAnterior(NodoHash* nodo);
double getMaxDelta(NodoHash* nodo);
int setMaxDelta(NodoHash* nodo, double delta);
double mediaJitter(NodoHash* nodo);
int setTiempoFin(NodoHash* nodo, double tiempo);
double getDelta(NodoHash* nodo) ;
int setDelta(NodoHash* nodo, double delta);
double mediaDelta(NodoHash* nodo) ;
int getBytesAnt(NodoHash* nodo);
int setBytesAnt(NodoHash* nodo, int bytes);
double mediaBytes(NodoHash* nodo);
double mediaAnchoBanda(NodoHash* nodo);
int insertarLista(NodoHash* nodo, char* clave,  char * destino, u_int16_t puerto_origen, u_int16_t puerto_destino,double tiempo,double jitter, double delta, double ancho_banda, double perdidas, double mos);
ListaH* getLista(NodoHash* nodo);
void printListaHash(TablaHash *tabla) ;
double getIntervalo(NodoHash* nodo) ;
int setIntervalo(NodoHash* nodo, double intervalo);
double mediaJitterIntervalo(NodoHash* nodo, double jitter, int recibidos);
int setJitterIntervalo(NodoHash* nodo, double jitter);
double getJitterIntervalo(NodoHash* nodo) ;
double getRecIntervalo(NodoHash* nodo);
int setRecibidosInt(NodoHash* nodo, int num);
int getRecibidosIntervalo(NodoHash* nodo);
double mediaDeltaIntervalo(NodoHash* nodo, double delta, int recibidos);
int setDeltaIntervalo(NodoHash* nodo, double delta);
double getDeltaIntervalo(NodoHash* nodo);
int setIntervaloNull(NodoHash* nodo);
double mediaAnchoBandaIntervalo(NodoHash* nodo, int bytes, double tini, double tfin) ;
int setBytesIntervalo(NodoHash* nodo, int bytes);
int getBytesIntervalo(NodoHash* nodo);
int setPorcentajePerdidas(NodoHash* nodo);
double getPorcentajePerdidas(NodoHash* nodo) ;
int setPerdidosInt(NodoHash* nodo, int num) ;
double setPorcentajePerdidasInt(NodoHash* nodo, int paq_rec, int paq_perd);
double getPorcentajePerdidasInt(NodoHash* nodo);
int getPerdidosIntervalo(NodoHash* nodo) ;
double getMOS(NodoHash* nodo) ;
double calcularMOS (NodoHash* nodo);
double calcularMOSInt (NodoHash* nodo, double paq_perd);

#endif
