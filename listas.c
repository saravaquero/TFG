#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "listas.h"

/**
 * Función que reserva memoria y crea un nodo
 *
 * Salida:
 *    nodo creado
 *
 */

Nodo * iniciaNodo(){
    Nodo * nodo=NULL;

    nodo=(Nodo *)malloc(sizeof(Nodo));
    if(nodo){
        nodo->siguiente=NULL;
        nodo->dato=NULL;
    }
    return nodo;
}

/**
 * Función que libera la memoria de un nodo
 *
 * Entrada:
 *    nodo a liberar
 *
 */

void borraNodo(Nodo * nodo){
  if(nodo){
        free(nodo->dato);
        free(nodo);
    }
}

/**
 * Función que crea una lista enlazada vacía y reserva memoria para el nodo
 * cabecera
 *
 * Salida:
 *    Estructura Lista con memoria asignada e inicializada
 */
Lista * crearLista(){

    Lista * lista=NULL;

    lista=(Lista *)malloc(sizeof(Lista));

    if(lista)
        lista->nodo=NULL;

    return lista;
}

/**
 * Comprueba si la lista esta vacía
 *
 * Entrada:
 *    lista: Lista que se quiere comprobar
 *
 * Salida:
 *    Devuelve 1 si la lista esta vacía y 0 si tiene al menos un elemento
 */
int esVacia(const Lista * lista){
    if(!lista)
        return 1;

    if(!lista->nodo)
        return 1;
    return 0;
}

/**
 * Extrae el primer elemento de la lista
 *
 * Entrada:
 *    lista: Lista que se quiere comprobar
 *
 * Salida:
 *    información del elemento
 */

char* extraePrimero(Lista * lista){
    Nodo * pn =NULL;
    char *pe = NULL;

    if(!lista || esVacia(lista)==1)
        return NULL;

    pn=lista->nodo;
    pe=(char*)malloc(sizeof(char)*(strlen(pn->dato)+1));

    if(!pe){
        return NULL;
    }
    strcpy(pe, pn->dato);
    lista->nodo=pn->siguiente;
    borraNodo(pn);

    return pe;
}

/**
 * Extrae el ultimo elemento de la lista
 *
 * Entrada:
 *    lista: Lista que se quiere comprobar
 *
 * Salida:
 *    información del elemento
 */
char* extraeUltimo(Lista * lista){
    Nodo * pn = NULL;
    char *pe = NULL;

    if(!lista || esVacia(lista))
        return NULL;

    /*Caso 1: Sólo un elemento en la lista*/
    if(!(lista->nodo)->siguiente){
        pe=(char*)malloc(sizeof(char)*(strlen(pn->dato)+1));

        if(!pe)
            return NULL;
        strcpy(pe, pn->dato);
        borraNodo(lista->nodo);
        lista->nodo=NULL;
        return pe;
    }

    /*Caso 2: Más de un elemento en la lista*/
    pn=lista->nodo;
    /*Se sitúa en el penúltimo lugar*/
    while(((pn->siguiente)->siguiente)!=NULL){
        pn=pn->siguiente;
    }
    pe=(char*)malloc(sizeof(char)*(strlen(pn->dato)+1));
    if(!pe)
        return NULL;
    strcpy(pe, pn->dato);
    borraNodo(pn->siguiente);
    pn->siguiente=NULL;
    return pe;
}

/**
 * Función que libera la memoria que se había reservado para los datos de la
 * lista
 *
 * Entrada:
 *    Estructura lista a liberar
 */
void borraLista(Lista * lista){
    if(!lista)
        return;

    while(esVacia(lista)==0)
        free(extraePrimero(lista));
    free(lista);
 }


 /**
  * Inserta elemento al principio de la lista
  *
  * Entrada:
  *    lista: Lista en la que se quiere insertar
  *    char*: elemento que queremos insertar
  *
  * Salida:
  *    lista modificada
  */
 Lista * insertaPrimero(Lista * lista, const char *elem){
    Nodo * pn;

    if(!lista || !elem)
        return NULL;

    pn=iniciaNodo();

    if(!pn)
        return NULL;

    pn->dato = (char*)malloc(sizeof(char)*(strlen(elem)+1));
    if(!pn->dato){
        borraNodo(pn);
        return NULL;
    }
    strcpy(pn->dato, elem);
    pn->siguiente=lista->nodo;
    lista->nodo=pn;

    return lista;
 }

 /**
  * Inserta elemento al final de la lista
  *
  * Entrada:
  *    lista: Lista en la que se quiere insertar
  *    char*: elemento que queremos insertar
  *
  * Salida:
  *    lista modificada
  */
  Lista * insertaFinal(Lista * lista, const char *elem){
    Nodo * pn, *naux;
    if(!lista || !elem)
        return NULL;

    pn=iniciaNodo();
    if(!pn)
        return NULL;

    pn->dato = (char*)malloc(sizeof(char)*(strlen(elem)+1));
    if(!pn->dato){
        borraNodo(pn);
        return NULL;
    }
    strcpy((pn->dato), elem);
    if(esVacia(lista)==1){
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

/**
  * Realizar una función denominada longitud que reciba como argumento una
  * lista e imprima en pantalla el número de elementos que contiene.
  **/

  int tamanoLista(const Lista * lista){
    Nodo * pn;
    int contador=0;
    if(!lista)
        return -1;

    if(esVacia(lista)==1)
        return 0;

    pn=lista->nodo;
    contador++;

    while((pn->siguiente)!=NULL){
        pn=pn->siguiente;
        contador++;
    }
    return contador;
  }

  /**
 * Función para imprimir la lista
 *
 * Entrada:
 *    lista: Lista a imprimir
 */
 int imprimeLista(const Lista * lista){
    Nodo * pn;
    int contador, aux;
    if(!lista)
        return -1;

    if(esVacia(lista))
        return 0;

    pn=lista->nodo;

    while(pn!=NULL){
        fprintf(stdout, "%s", pn->dato);
        contador+=(aux+1);
        pn=pn->siguiente;
    }
    fprintf(stdout,"\n");
    return contador;

 }

 /**
 * Devuelve un puntero al nodo que contiene X como dato; NULL si no lo encuentra
 *
 * Entrada:
 *    ip: Elemento a buscar
 *    lista: Puntero a la lista
 */
 int existeElemento(const Lista * lista, char* ip){
    Nodo * pn= NULL;
    char *pe = NULL;
    int contador, aux;

    if(!ip || !lista)
        return 0;

    if(esVacia(lista))
        return 0;

    pn = lista->nodo;

    while(pn!=NULL){

        pe = pn->dato;
        if(strcmp(pe, ip) == 0){
            return 1;
        }
        pn = pn->siguiente;
    }

    return 0;
 }

 /**
  * Borra el nodo de la Lista * l que contiene el dato X y no hace nada si X no
  * está en la lista enlazada
  *
  * Entrada:
  *    X: Dato del que se quiere borrar el nodo
  *    L: Lista de trabajo
  */
 int borrarElemento(Lista * lista, char* ip){

   Nodo * pn=NULL;
   Nodo * pa =NULL;
   char *pe = NULL;

   if(!lista || esVacia(lista)==1)
       return 0;

   pn=lista->nodo;

   pe = pn->dato;
   if(strcmp(pe, ip) == 0){
       pe = extraePrimero(lista);
       if(pe == NULL)
           return 0;
       free(pe);
       return 1;
   }

   pa = pn;
   while(pn != NULL){
       pe = pn->dato;
       if(strcmp(pe, ip) == 0){
           if(pn->siguiente == NULL)
               pa->siguiente = NULL;
           else
               pa->siguiente = pn->siguiente;
           borraNodo(pn);
           return 1;
       }
       pa = pn;
       pn = pa->siguiente;
   }

   return 0;
 }


 /**
  * Copia la lista1 en la lista2
  *
  *
  * Entrada:
  *    lista1: lista que queremos copiar
  *    lista2: Lista donde la copiaremos
  *
  * Salida: 1 si se copio correctamente 0 si no
  */
int copiaLista(Lista * lista1, Lista * lista2){
  Nodo * pn=NULL;
  Nodo * pa =NULL;

   if(lista1 == NULL || lista2 == NULL)
       return 0;

   pn = lista1->nodo;
   while(pn != NULL){
       if(insertaPrimero(lista2, pn->dato) == NULL)
           return 0;
       pa = pn;
       pn = pa->siguiente;
   }

   return 1;
}



char** ipClientes(const Lista * lista){
    int i, j, size;
    char **clientes;
    Nodo * pn =NULL;
    char * pe = NULL;

    if(!lista || esVacia(lista))
        return NULL;

    size = tamanoLista(lista);
    clientes = (char**)malloc(size*sizeof(char*));
    if(!clientes){
        fprintf(stderr, "Error al obtener los clientes de un nodo Hash");
        return NULL;
    }
    pn = lista->nodo;

    for(i = 0; i < size; i++){

        pe = pn->dato;
        clientes[i] = pe;
        pn = pn->siguiente;
    }

    return clientes;
}
