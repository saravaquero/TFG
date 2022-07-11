#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "listaRTP.h"


Nodo*NodoRTP_ini(){
    Nodo*nodo = NULL;

    nodo=(Nodo*)malloc(sizeof(Nodo));
    if(nodo){
        nodo->siguiente=NULL;
        nodo->clave=NULL;
        nodo->destino=NULL;
        nodo->sport=0;
        nodo->dport=0;
        nodo->tiempo=0.0;
        nodo->info=0;
    }
    return nodo;
}


void borra_nodoRTP(Nodo*nodo){
    if(nodo){
        free(nodo->clave);
        free(nodo->destino);
        free(nodo);
    }
}


ListaRTP *crea_ListaRTP(){
    ListaRTP *lista=NULL;

    lista=(ListaRTP *)malloc(sizeof(ListaRTP));

    if(lista)
        lista->nodo=NULL;

    return lista;
}

 void borra_ListaRTP(ListaRTP *lista){
    if(!lista)
        return;

    while(esVacia_ListaRTP(lista)==0)
        free(ExtraePrimero_ListaRTP(lista));
    free(lista);
 }


 ListaRTP *InsertaPrimero_ListaRTP(ListaRTP *lista, const char *clave, const char *destino,const uint16_t puerto_origen, const uint16_t puerto_destino, const double tiempo, const int info){
    Nodo*pn;

    if(!lista || !clave || !destino)
        return NULL;

    pn=NodoRTP_ini();

    if(!pn)
        return NULL;

    pn->clave = (char*)malloc(sizeof(char)*(strlen(clave)+1));
    if(!pn->clave){
        borra_nodoRTP(pn);
        return NULL;
    }

    pn->destino = (char*)malloc(sizeof(char)*(strlen(destino)+1));
    if(!pn->destino){
        borra_nodoRTP(pn);
        return NULL;
    }


    strcpy(pn->clave, clave);
    strcpy(pn->destino, destino);
    pn->dport = puerto_destino;
    pn->sport = puerto_origen;
    pn->tiempo = tiempo;
    pn->info = info;
    pn->siguiente=lista->nodo;
    lista->nodo=pn;

    return lista;
 }


ListaRTP *InsertaFinal_ListaRTP(ListaRTP *lista, const char *clave, const char *destino,const uint16_t puerto_origen, const uint16_t puerto_destino, const double tiempo, const int info){
    Nodo*pn, *naux;
    if(!lista || !clave)
        return NULL;

    pn=NodoRTP_ini();
    if(!pn)
        return NULL;

    pn->clave = (char*)malloc(sizeof(char)*(strlen(clave)+1));
    if(!pn->clave){
        borra_nodoRTP(pn);
        return NULL;
    }
    strcpy(pn->clave, clave);

    pn->destino = (char*)malloc(sizeof(char)*(strlen(destino)+1));
    if(!pn->destino){
        borra_nodoRTP(pn);
        return NULL;
    }

    strcpy(pn->destino, destino);

    pn->sport=puerto_origen;

    pn->dport=puerto_destino;

    pn->tiempo = tiempo;
    pn->info = info;
    if(esVacia_ListaRTP(lista)==1){
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


char* ExtraePrimero_ListaRTP(ListaRTP *lista){
    Nodo*pn =NULL;
    char *pe = NULL;

    if(!lista || esVacia_ListaRTP(lista)==1)
        return NULL;

    pn=lista->nodo;
    pe=(char*)malloc(sizeof(char)*(strlen(pn->clave)+1));
    if(!pe){
        return NULL;
    }
    strcpy(pe, pn->clave);


    lista->nodo=pn->siguiente;
    borra_nodoRTP(pn);

    return pe;
}

/***************************************************************************
* @Title: lisRTPt_extractLast
* @Description: extrae el nodo de la ultima posicion devolviendo la direccion del clave.
* @Input: ListaRTP **: Puntero a la lista a extraer.
* @Output: char *: direccion del clave extraido
***************************************************************************/
char* ExtraeFinal_ListaRTP(ListaRTP *lista){
    Nodo*pn = NULL;
    char *pe = NULL;

    if(!lista || esVacia_ListaRTP(lista))
        return NULL;

    /*Caso 1: Sólo un elemento en la lista*/
    if(!(lista->nodo)->siguiente){
        pe=(char*)malloc(sizeof(char)*(strlen(pn->clave)+1));

        if(!pe)
            return NULL;
        strcpy(pe, pn->clave);
        borra_nodoRTP(lista->nodo);
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
    borra_nodoRTP(pn->siguiente);
    pn->siguiente=NULL;
    return pe;
}


int esVacia_ListaRTP(const ListaRTP *lista){
    if(!lista)
        return 1;

    if(!lista->nodo)
        return 1;
    return 0;
}


int tam_ListaRTP(const ListaRTP *lista){
    Nodo*pn;
    int contador=0;
    if(!lista)
        return -1;

    if(esVacia_ListaRTP(lista)==1)
        return 0;

    pn=lista->nodo;
    contador++;

    while(pn->siguiente!=NULL){
        pn=pn->siguiente;
        contador++;
    }
    return contador;
}



int imprime_ListaRTP(const ListaRTP *lista){
    Nodo*pn;
    int contador, aux;
    if(!lista)
        return -1;

    fprintf(stdout, "\nLista con %d elementos:\n", tam_ListaRTP(lista));
    if(esVacia_ListaRTP(lista))
        return 0;

    pn=lista->nodo;

    while(pn!=NULL){
        fprintf(stdout, "%s", pn->clave);
        fprintf(stdout, "%s", pn->destino);
        fprintf(stdout, " %d", pn->dport);
        fprintf(stdout, " %d", pn->sport);
        fprintf(stdout, " %f", pn->tiempo);
        fprintf(stdout, " %d", pn->info);
        fprintf(stdout,"\n");
        contador+=(aux+1);
        pn=pn->siguiente;
    }
    return contador;
}

/****************************************************************************
* @Title: existeElemento_ListaRTP
* @Description: Comprueba si un clave esta en lista.
* @Input:  ListaRTP **: Puntero a la lista.
*          char*: clave a buscar.
* @Output: int: 1 si se encuentra, 0 si no.
*****************************************************************************/
int existeElemento_ListaRTP(const ListaRTP *lista, char* ip, char * destino, uint16_t puerto_origen, uint16_t puerto_destino){
    Nodo*pn = NULL;
    char *pe = NULL;
    char *pi = NULL;
    uint16_t d=0;
    uint16_t s=0;
    int contador, aux;

    if(!ip || !lista)
        return 0;

    if(esVacia_ListaRTP(lista))
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

/****************************************************************************
* @Title: extraeElemento_ListaRTP
* @Description: extraemos de la lista el clave pasado por argumento.
* @Input:  ListaRTP **: Puntero a la lista.
*          char*: clave a extraer.
* @Output: int: 1 si se encuentra, 0 si no.
*****************************************************************************/
int extraeElemento_ListaRTP(ListaRTP *lista, char* ip){

    Nodo*pn =NULL;
    Nodo*pa =NULL;
    char *pe = NULL;

    if(!lista || esVacia_ListaRTP(lista)==1)
        return 0;

    pn=lista->nodo;

    pe = pn->clave;
    if(strcmp(pe, ip) == 0){
        pe = ExtraePrimero_ListaRTP(lista);
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
            borra_nodoRTP(pn);
            return 1;
        }
        pa = pn;
        pn = pa->siguiente;
    }

    return 0;
}



char* getclave(Nodo*nodo) {

    if(nodo == NULL || nodo->clave == NULL)
        return NULL;

    return nodo->clave;
}


double getTiempo(Nodo*nodo) {

    if(nodo == NULL)
        return -1;

    return nodo->tiempo;
}


int setTiempo(Nodo*nodo, double tmp) {
    if(nodo == NULL || tmp < 0)
        return -1;

    nodo->tiempo = tmp;
    return 0;
}


int getInfo(Nodo*nodo) {

    if(nodo == NULL)
        return -1;

    return nodo->info;
}


int setInfo(Nodo*nodo, int inf) {
    if(nodo == NULL || inf < 0)
        return -1;

    nodo->info = inf;
    return 0;
}


Nodo*getNext(Nodo*nodo) {

    if(nodo == NULL)
        return NULL;

    return nodo->siguiente;
}

/****************************************************************************
* @Title: getNodo
* @Description: devolvemos el nodo que contenga el mismo nombre
* @Input:  ListaRTP **: puntero a la lista.
           char*: nombre del clave a buscar.
* @Output: NodoRTP
*****************************************************************************/
Nodo*getNodo(ListaRTP *lista, char* clave, char* destino, uint16_t puerto_origen, uint16_t puerto_destino) {

    Nodo*pn =NULL;
    Nodo*pa =NULL;
    char *pe = NULL;
    char *pi = NULL;
    uint16_t d=0;
    uint16_t s=0;

    if(!lista || esVacia_ListaRTP(lista)==1)
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


Nodo*getNodoPos(ListaRTP *lista, int pos) {

    Nodo*pn =NULL;
    Nodo*pa =NULL;
    int i = 0;

    if(!lista || esVacia_ListaRTP(lista)==1)
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

int cambiarInfo(Nodo*nodo, const int info){
  if(nodo == NULL)
      return 0;

  nodo->info = info;

  return 1;

}
