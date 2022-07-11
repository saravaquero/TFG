#ifndef __LISTAS_H_
#define __LISTAS_H_



/* Estructura del TAD Lista *enlazada y nodo de la lista*/

typedef struct _Nodo{
	char * dato;
	 struct _Nodo *siguiente;
}Nodo;

typedef struct List Lista;
struct List{
    Nodo * nodo;
};



/*******FUNCIONES*****/

// crea un nodo
Nodo * iniciaNodo();

//Borra un nodo
void borraNodo(Nodo * nodo);
/* Crea la Lista **/
//List* list_ini();
Lista *crearLista();

/* Comprueba si una Lista *está vacía o no. */
//int list_isEmpty(const List* list);
int esVacia(const Lista *lista);

/* Libera la Lista **/
//void free_list(List* list);
void borraLista(Lista *lista);

/* Extrae del principio de la Lista *realizando una copia del elemento almacenado en dicho nodo. */
//char* list_extractFirst(List* list);
char* extraePrimero(Lista *lista);

/* Extrae del final de la Lista *realizando una copia del elemento almacenado en dicho nodo. */
char* extraeUltimo(Lista *lista);

/* Inserta al principio de la Lista *realizando una copia del elemento. */
Lista *insertaPrimero(Lista *lista, const char *elem);

/* Inserta al final de la Lista *realizando una copia del elemento. */
Lista *insertaFinal(Lista *lista, const char *elem);


/* Devuelve el tamaño de una lista. */
int tamanoLista(const Lista *lista);

/* Imprime el contenido de la lista. */
int imprimeLista(const Lista *lista);

/* Comprueba si el elemento esta en la lista. */
int existeElemento(const Lista *lista, char* ip);

/* Eliminamos el nodo de la Lista *que contenga el elemento*/
int borrarElemento(Lista *lista, char* ip);

/* Devuelve la ip de todos los clientes de la Lista **/
char** ipClientes(const Lista *lista);

/* Copiamos la Lista *1 en la Lista *2*/
int copiaLista(Lista *lista1, Lista *lista2);

#endif
