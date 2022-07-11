#ifndef LISTARTP_H
#define LISTARTP_H

typedef struct Lista_RTP ListaRTP;
typedef struct Nodo_RTP Nodo;

struct Nodo_RTP{
    char* clave;                    // Ip del clave
    char* destino;
    uint16_t sport;
    uint16_t dport;
    double tiempo;                  // Ultimo tiempo de llegada
    int info;
    struct Nodo_RTP *siguiente;
};

struct Lista_RTP{
    Nodo * nodo;
};
/********************* Declaracion de funciones públicas **********************/

Nodo *Nodo_ini();

void Nodo_free(Nodo *nodo);
/* Inicializa la listaa */
ListaRTP *crea_ListaRTP();

/* Libera la listaa */
void borra_ListaRTP(ListaRTP *lista);

/* Inserta al principio de la listaa realizando una copia de los elemento. */
ListaRTP *InsertaPrimero_ListaRTP(ListaRTP *lista, const char *clave, const char *destino,const uint16_t puerto_origen, const uint16_t puerto_destino, const double tiempo, const int info);
/* Inserta al final de la listaa realizando una copia de los elemento. */
ListaRTP *InsertaFinal_ListaRTP(ListaRTP *lista, const char *clave, const char *destino,const uint16_t puerto_origen, const uint16_t puerto_destino, const double tiempo, const int info);
/* Extrae del principio de la listaa realizando una copia del clave. */
char* ExtraePrimero_ListaRTP(ListaRTP *lista);

/* Extrae del final de la listaa realizando una copia del clave. */
char* ExtraeFinal_ListaRTP(ListaRTP *lista);

/* Comprueba si la listaa está vacía o no. */
int esVacia_ListaRTP(const ListaRTP *lista);

/* Devuelve el tamaño de la listaa. */
int tam_ListaRTP(const ListaRTP *lista);

/* Imprime una listaa devolviendo el tamanio. */
int imprime_ListaRTP(const ListaRTP *lista);

/* Comprueba si el clave esta en la listaa. */
int existeElemento_ListaRTP(const ListaRTP *lista, char* ipconst , char *destino, uint16_t puerto_origen, uint16_t puerto_destino);

/* Eliminamos el nodo que contiene el clave de la listaa*/
int extraeElemento_ListaRTP(ListaRTP *lista, char* ip);

/* Devolvemos la ip del clave del nodo*/
char* getclave(Nodo *nodo);

/* Devolvemos eltiempo de llegada guardado en el nodo*/
double getTiempo(Nodo *nodo);

/* Modificamos eltiempo de llegada guardado en el nodo*/
int setTiempo(Nodo *nodo, double tmp);

/* Devolvemos la informacion guardada en el nodo*/
int getInfo(Nodo *nodo);

/* Modificamos la informacion guardada en el nodo*/
int setInfo(Nodo *nodo, int inf);

/* Devolvemos el nodo vecino */
Nodo *getNext(Nodo *nodo);

/* Devulve el nodo que tenga la ip */
Nodo *getNodo(ListaRTP *lista, char* clave, char* destino, uint16_t puerto_origen,uint16_t puerto_destino);

/* Devuelve un nodo dado una posicion */
Nodo *getNodePos(ListaRTP *lista, int pos);

int cambiarInfo(Nodo *nodo, const int info);

/* Se realiza una copia de la listaa */
//ListaRTP *ListaRTP_copy(const ListaRTP *lista);

#endif /*ListaRTP_H*/
