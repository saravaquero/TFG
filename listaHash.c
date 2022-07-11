#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "listaHash.h"


NodoH*NodoH_ini(){
    NodoH * nodo = NULL;

    nodo=(NodoH*)malloc(sizeof(NodoH));
    if(nodo){
        nodo->siguiente=NULL;
        nodo->clave=NULL;
        nodo->destino=NULL;
        nodo->sport=0;
        nodo->dport=0;
        nodo->tiempo=0.0;
        nodo->jitter=0.0;
        nodo->ancho_banda=0.0;
        nodo->delta=0.0;
        nodo->perdidas=0.0;
        nodo->mos=0.0;

    }
    return nodo;
}


void borra_nodoH(NodoH*nodo){
    if(nodo){
        free(nodo->clave);
        free(nodo->destino);
        free(nodo);
    }
}


ListaH *crea_ListaH(){
    ListaH *lista=NULL;

    lista=(ListaH *)malloc(sizeof(ListaH));

    if(lista)
        lista->nodo=NULL;

    return lista;
}

 void borra_ListaH(ListaH *lista){
    if(!lista)
        return;

    while(esVacia_ListaH(lista)==0)
        free(ExtraePrimero_ListaH(lista));
    free(lista);
 }


   ListaH *InsertaPrimero_ListaH(ListaH *lista, const char *clave, const char *destino,const int puerto_origen, const int puerto_destino, const double tiempo, const double jitter, const double delta, const double ancho_banda, const double perdidas, const double mos){
    NodoH*pn;

    if(!lista || !clave || !destino)
        return NULL;

    pn=NodoH_ini();

    if(!pn)
        return NULL;

    pn->clave = (char*)malloc(sizeof(char)*(strlen(clave)+1));
    if(!pn->clave){
        borra_nodoH(pn);
        return NULL;
    }

    pn->destino = (char*)malloc(sizeof(char)*(strlen(destino)+1));
    if(!pn->destino){
        borra_nodoH(pn);
        return NULL;
    }


    strcpy(pn->clave, clave);
    strcpy(pn->destino, destino);
    pn->dport = puerto_destino;
    pn->sport = puerto_origen;
    pn->tiempo = tiempo;
    pn->jitter = jitter;
    pn->delta = delta;
    pn->ancho_banda= ancho_banda;
    pn->perdidas= perdidas;
    pn->mos= mos;
    pn->siguiente=lista->nodo;
    lista->nodo=pn;

    return lista;
 }


ListaH *InsertaFinal_ListaH(ListaH *lista, const char *clave, const char *destino,const int puerto_origen, const int puerto_destino, const double tiempo, const double jitter, const double delta,  const double ancho_banda, const double perdidas, const double mos){
    NodoH*pn, *naux;
    if(!lista || !clave)
        return NULL;

    pn=NodoH_ini();
    if(!pn)
        return NULL;

    pn->clave = (char*)malloc(sizeof(char)*(strlen(clave)+1));
    if(!pn->clave){
        borra_nodoH(pn);
        return NULL;
    }
    strcpy(pn->clave, clave);

    pn->destino = (char*)malloc(sizeof(char)*(strlen(destino)+1));
    if(!pn->destino){
        borra_nodoH(pn);
        return NULL;
    }

    strcpy(pn->destino, destino);

    pn->sport=puerto_origen;

    pn->dport=puerto_destino;

    pn->tiempo = tiempo;
    pn->delta = delta;
    pn->jitter=jitter;
    pn->ancho_banda= ancho_banda;
    pn->perdidas= perdidas;
    pn->mos= mos;
    if(esVacia_ListaH(lista)==1){
        lista->nodo=pn;
        return lista;
    }

    naux=lista->nodo;
    while(naux->siguiente!=NULL){
        naux=naux->siguiente;
    }
    naux->siguiente=pn;
    return lista;
}


char* ExtraePrimero_ListaH(ListaH *lista){
    NodoH*pn =NULL;
    char *pe = NULL;

    if(!lista || esVacia_ListaH(lista)==1)
        return NULL;

    pn=lista->nodo;
    pe=(char*)malloc(sizeof(char)*(strlen(pn->clave)+1));
    if(!pe){
        return NULL;
    }
    strcpy(pe, pn->clave);


    lista->nodo=pn->siguiente;
    borra_nodoH(pn);

    return pe;
}


char* ExtraeFinal_ListaH(ListaH *lista){
    NodoH*pn = NULL;
    char *pe = NULL;

    if(!lista || esVacia_ListaH(lista))
        return NULL;

    /*Caso 1: Sólo un elemento en la lista*/
    if(!(lista->nodo)->siguiente){
        pe=(char*)malloc(sizeof(char)*(strlen(pn->clave)+1));

        if(!pe)
            return NULL;
        strcpy(pe, pn->clave);
        borra_nodoH(lista->nodo);
        lista->nodo=NULL;
        return pe;
    }

    /*Caso 2: Más de un elemento en la lista*/
    pn=lista->nodo;
    /*Se sitúa en el penúltimo lugar*/
    while(((pn->siguiente)->siguiente)!=NULL){
        pn=pn->siguiente;
    }
    pe=(char*)malloc(sizeof(char)*(strlen(pn->clave)+1));
    if(!pe)
        return NULL;
    strcpy(pe, pn->clave);
    borra_nodoH(pn->siguiente);
    pn->siguiente=NULL;
    return pe;
}


int esVacia_ListaH(const ListaH *lista){
    if(!lista)
        return 1;

    if(!lista->nodo)
        return 1;
    return 0;
}


int tam_ListaH(const ListaH *lista){
    NodoH*pn;
    int contador=0;
    if(!lista)
        return -1;

    if(esVacia_ListaH(lista)==1)
        return 0;

    pn=lista->nodo;
    contador++;

    while(pn->siguiente!=NULL){
        pn=pn->siguiente;
        contador++;
    }
    return contador;
}



int imprime_ListaH(const ListaH *lista){
    NodoH*pn;
    int contador, aux;
    if(!lista)
        return -1;

    fprintf(stdout, "\nLista con %d elementos:\n", tam_ListaH(lista));
    if(esVacia_ListaH(lista))
        return 0;

    pn=lista->nodo;

    while(pn!=NULL){
        fprintf(stdout,"Origen\t Destino\t Puerto origen\t Puerto destino\t tiempo\t delta\t jitter\t ancho_banda\t perdidas\n");
        fprintf(stdout, "%s\t", pn->clave);
        fprintf(stdout, "%s\t", pn->destino);
        fprintf(stdout, " %d\t", pn->dport);
        fprintf(stdout, " %d\t", pn->sport);
        fprintf(stdout, " %f\t", pn->tiempo);
        fprintf(stdout, " %f\t", pn->delta);
        fprintf(stdout, " %f\t", pn->jitter);
        fprintf(stdout, " %f\t", pn->ancho_banda);
        fprintf(stdout, " %f\t", pn->perdidas);
        fprintf(stdout, " %f\t", pn->mos);
        fprintf(stdout,"\n");
        fprintf(stdout,"\n");
        contador+=(aux+1);
        pn=pn->siguiente;
    }
    return contador;
}

int existeElemento_ListaH(const ListaH *lista, char* ip, char * destino, int puerto_origen, int puerto_destino){
    NodoH*pn = NULL;
    char *pe = NULL;
    char *pi = NULL;
    int d=0;
    int s=0;
    int contador, aux;

    if(!ip || !lista)
        return 0;

    if(esVacia_ListaH(lista))
        return 0;

    pn = lista->nodo;

    while(pn!=NULL){

        pe = pn->clave;
        pi = pn->destino;
        d=pn->dport;
        s=pn->sport;
        if(strcmp(pe, ip) == 0 && strcmp(pi, destino) == 0 && d== puerto_destino && s==puerto_origen){
            return 1;
        }
        pn = pn->siguiente;
    }

    return 0;
}


int extraeElemento_ListaH(ListaH *lista, char* ip){

    NodoH*pn =NULL;
    NodoH*pa =NULL;
    char *pe = NULL;

    if(!lista || esVacia_ListaH(lista)==1)
        return 0;

    pn=lista->nodo;

    pe = pn->clave;
    if(strcmp(pe, ip) == 0){
        pe = ExtraePrimero_ListaH(lista);
        if(pe == NULL)
            return 0;
        free(pe);
        return 1;
    }

    pa = pn;
    while(pn != NULL){
        pe = pn->clave;
        if(strcmp(pe, ip) == 0){
            if(pn->siguiente == NULL)
                pa->siguiente = NULL;
            else
                pa->siguiente = pn->siguiente;
            borra_nodoH(pn);
            return 1;
        }
        pa = pn;
        pn = pa->siguiente;
    }

    return 0;
}



char* getHclave(NodoH*nodo) {

    if(nodo == NULL || nodo->clave == NULL)
        return NULL;

    return nodo->clave;
}

char* getHDestino(NodoH*nodo) {

  if(nodo == NULL || nodo->destino == NULL)
      return NULL;

    return nodo->destino;
}

int getH_dport(NodoH*nodo) {

  if(nodo== NULL)
      return -1;

    return nodo->dport;
}

int getH_sport(NodoH*nodo) {

  if(nodo== NULL)
      return -1;

    return nodo->sport;
}


double getHTiempo(NodoH*nodo) {

    if(nodo== NULL)
        return -1;

    return nodo->tiempo;
}



double getHJitter(NodoH*nodo) {

    if(nodo== NULL)
        return -1;

    return nodo->jitter;
}

double getHPerdidas(NodoH*nodo) {

    if(nodo== NULL)
        return -1;

    return nodo->perdidas;
}

double getHMOS(NodoH*nodo) {

    if(nodo== NULL)
        return -1;

    return nodo->mos;
}


double getHAnchoBanda(NodoH*nodo) {

    if(nodo== NULL)
        return -1;

    return nodo->ancho_banda;
}

double getHDelta(NodoH*nodo) {

    if(nodo== NULL)
        return -1;

    return nodo->delta;
}





int setHTiempo(NodoH*nodo, double tmp) {
    if(nodo== NULL || tmp < 0)
        return -1;

    nodo->tiempo = tmp;
    return 0;
}



NodoH*getHNext(NodoH*nodo) {

    if(nodo== NULL)
        return NULL;

    return nodo->siguiente;
}

NodoH*getHNodo(ListaH *lista, char* clave, char* destino, int puerto_origen, int puerto_destino) {

    NodoH*pn =NULL;
    NodoH*pa =NULL;
    char *pe = NULL;
    char *pi = NULL;
    int d=0;
    int s=0;

    if(!lista || esVacia_ListaH(lista)==1)
        return NULL;

    pn=lista->nodo;

    pe = pn->clave;
    pi = pn->destino;
    d=pn->dport;
    s=pn->sport;
    if(strcmp(pe, clave) == 0 &&  strcmp(pi, destino) == 0 && d== puerto_destino && s==puerto_origen){
        return pn;
    }

    pa = pn;
    while(pn != NULL){
        pe = pn->clave;
        pi = pn->destino;
        d=pn->dport;
        s=pn->sport;
        if(strcmp(pe, clave) == 0 &&  strcmp(pi, destino) == 0 && d== puerto_destino && s==puerto_origen){
            return pn;
        }
        pa = pn;
        pn = pa->siguiente;
    }

    return NULL;

}


NodoH*getHNodoPos(ListaH *lista, int pos) {

    NodoH*pn =NULL;
    NodoH*pa =NULL;
    int i = 0;

    if(!lista || esVacia_ListaH(lista)==1)
        return NULL;

    pn=lista->nodo;

    if(pos == 0)
        return pn;

    while(i < pos) {

        pa = pn;
        pn = pa->siguiente;
        if(pn == NULL)
            return NULL;
        i++;
    }

    return pn;
}
