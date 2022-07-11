#include "hash.h"


struct Nodo_Hash {
	char origen[MAXNUM];    	/*IP origen */
	char destino[MAXNUM];		/*IP destino */
	int dport;	/*puerto UDP destino*/
	int sport;	/*puerto UDP origen*/
	int payload;
	u_int32_t ssrc;
	int paq_rec;					/*Paquetes Recibidos */
	int paq_perd;					/*Paquetes Perdidos */
	int paq_rec_intervalo;
	int paq_perd_intervalo;
	double retardo;
	double retardoAnt;
	int bytes;
	int bytes_intervalo;
	int bytes_ant;
	NodoHash *siguiente;
	double jitter;
	double jitter_intervalo;
	double delta_intervalo;
	double jitterant;
	double maxjitter;
	double delta;
	double maxdelta;
	u_int32_t timeAnterior;
	double tini;     /*tiempo inicio*/
	double tfin;		/*tiempo final*/
	double media_ancho_banda;
	double media_delta;
	double media_jitter;
	double media_bytes;
	double intervalo;
	double porcentaje_perd;
	double mos;
	ListaH *lista;

};


/*Función que crea una tabla hash de un tamaño determinado
**Entrada: tamaño de la tabla
**Salida: tabla hash creada
*/

TablaHash* crearTablaHash(int tam) {
	TablaHash* hash = NULL;
	int i;

	hash = (TablaHash*) malloc(sizeof(TablaHash));
	if (hash == NULL)
		return NULL;

	hash->tam = tam;
	hash->nodos = (NodoHash**) malloc(sizeof(NodoHash*) * tam);
	if (hash->nodos == NULL) {
		free(hash);
		return NULL;
	}

	for (i = 0; i < tam; i++)
		hash->nodos[i] = NULL;

	return hash;
}

/*Función que elimina un nodo de forma recursiva
**Entrada: nodo que queremos borrar
**Salida: devuelve -1 si no se borra y 0 si se realiza correctamente
*/
int borrarNodoRec(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	if (nodo->siguiente != NULL)
		borrarNodoRec(nodo->siguiente);

	borra_ListaH(nodo->lista);
	free(nodo);

	return 0;
}


/*Función que elimina un nodo
**Entrada: nodo que queremos borrar
**Salida: devuelve -1 si no se borra y 0 si se realiza correctamente
*/

int borrarNodo(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	borrarNodoRec(nodo);

	return 0;
}




/*Función que elimina una tabla hash
**Entrada: tabla que queremos borrar
**Salida: devuelve -1 si no se borra y 0 si se realiza correctamente
*/

int borrarTablaHash(TablaHash *tabla) {
	int i;

	if (tabla == NULL)
		return -1;

	for (i = 0; i < tabla->tam; i++)
		borrarNodo(tabla->nodos[i]);

	free(tabla->nodos);
	free(tabla);

	return 0;
}

/*Devuelve la posicionicion de un nodo en la tabla hash
**Entrada: Tabla hash, ORIGEN, DESTINO, PUERTOS UDP ORIGEN Y DESTINO
**Salida: devuelve la posicionicion del elemento
*/

int funcionHash(TablaHash* tabla, char *origen, char *destino, int source_port, int dst_port) {

	int funcion;
	struct in_addr addr_origen;
	struct in_addr addr_destino;

	if(inet_aton(origen, &addr_origen) == 0)
		return -1;

	if(inet_aton(destino, &addr_destino) == 0)
		return -1;

	funcion= (addr_origen.s_addr + addr_destino.s_addr + source_port + dst_port)% tabla->tam;

	return funcion;
}

/*Devuelve el nodo hash y reserva memoria
**Entrada: origen, destino y puertos udp
**Salida: devuelve el nodo creado
*/

NodoHash* crearNodoHash(char *origen, char *destino, int puerto_origen, int puerto_destino) {
	NodoHash* nodo =  NULL;

	nodo = (NodoHash*) malloc(sizeof(NodoHash));
	if (nodo == NULL)
		return NULL;

	strcpy(nodo->origen, origen);
	strcpy(nodo->destino, destino);

	nodo->lista = crea_ListaH();
	if(nodo->lista == NULL)
		return NULL;



	nodo->dport = puerto_destino;
	nodo->sport = puerto_origen;
	nodo->paq_rec = 0;
	nodo->paq_perd = 0;
	nodo->porcentaje_perd= 0.0;
	nodo->paq_rec_intervalo = 0;
	nodo->paq_perd_intervalo = 0;
	nodo->ssrc = 0;
	nodo->payload = 0;
	nodo->retardo= 0.0;
	nodo->jitter = 0.0;
	nodo->jitter_intervalo = 0.0;
	nodo->delta = 0.0;
	nodo->delta_intervalo = 0.0;
	nodo->retardoAnt = 0.0;
	nodo->jitterant = 0.0;
	nodo->timeAnterior = 0.0;
	nodo->maxjitter = 0.0;
	nodo->maxdelta = 0.0;
	nodo->media_delta = 0.0;
	nodo->media_jitter = 0.0;
	nodo->media_ancho_banda=0.0;
	nodo->bytes = 0;
	nodo->bytes_intervalo = 0;
	nodo->media_bytes = 0.0;
	nodo->bytes_ant = 0;
	nodo->tini = 0.0;
	nodo->tfin = 0.0;
	nodo->intervalo = 0.0;
	nodo->mos=0.0;
	nodo->siguiente = NULL;

	return nodo;
}

/*Inserta un nodo en la tabla hash en la posicionicion que indique la funcion hash
**Entrada:tabla, origen, destino, puertos udp, informacion del nodos, ssrc, payload, tiempo, timestamp, bytes
		- tiempo
**Salida: devuelve -1 si no se inserta y 0 si se realiza correctamente
*/

int insertarNodoHash(TablaHash *tabla, char *origen, char *destino, void *info, int puerto_origen, int puerto_destino, u_int32_t ssrc, int payload, double tiempo, u_int32_t timestamp, int bytes) {
	NodoHash* nodo;
	NodoHash* aux;
	int posicion;

	if(tabla == NULL || origen == NULL || destino == NULL || puerto_origen == 0 || puerto_destino== 0)
		return -1;

	nodo = crearNodoHash(origen, destino, puerto_origen, puerto_destino);

	nodo->ssrc = ssrc;
	nodo->payload= payload;
	nodo->retardoAnt = tiempo;
	nodo->timeAnterior = timestamp;
	nodo->tini=tiempo/1000;
	nodo->bytes_ant = bytes;

	posicion = funcionHash(tabla, origen, destino, puerto_origen, puerto_destino);

	if (tabla->nodos[posicion] != NULL) {
		aux = tabla->nodos[posicion];
		while(aux->siguiente != NULL)
			aux = aux->siguiente;

		aux->siguiente = nodo;

	} else
		tabla->nodos[posicion] = nodo;

	return 0;
}

/*Busca un nodo en la tabla hash
**Entrada: Tabla, origen, destino, puertos udp
**Salida: se devuelve el contenido del nodo
*/
NodoHash* buscarNodoHash(TablaHash *tabla, char *origen, char *destino, int source_port, int dst_port) {

	NodoHash* aux;
	int posicion;

	posicion = funcionHash(tabla, origen, destino, source_port, dst_port);

	if(tabla->nodos[posicion] == NULL)
		return NULL;

	for (aux = tabla->nodos[posicion]; aux->siguiente != NULL; aux = aux->siguiente)
		if(strcmp(aux->origen, origen) == 0 && strcmp(aux->destino, destino)==0 && aux->sport == source_port && aux->dport == dst_port)
			break;

	//Si no encontramos el nodo, devolvemos NULL
	if(strcmp(aux->origen, origen) != 0 || strcmp(aux->destino, destino) != 0 && aux->sport != source_port && aux->dport !=dst_port)
		return NULL;

	return aux;
}



/*devuelve direccion origen
**Entrada: nodo
**Salida: origen
*/

char* getOrigen(NodoHash* nodo) {

	if(nodo == NULL)
		return NULL;

	return nodo->origen;
}


/*devuelve direccion destino
**Entrada: nodo
**Salida: origen
*/

char* getDestino(NodoHash* nodo) {

	if(nodo == NULL)
		return NULL;

	return nodo->destino;
}

/*devuelve el numero de paquetes recibidos
**Entrada: nodo
**Salida: numero de paquetes recibidos
*/

int getPuertoDestino(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->dport;
}

int getPuertoOrigen(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->sport;
}

int getPayload(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->payload;
}

int getSSRC(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->ssrc;
}


int getNumRecibidos(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->paq_rec;
}

ListaH* getLista(NodoHash* nodo){
	return nodo->lista;
}

int insertarLista(NodoHash* nodo, char* clave,  char * destino, u_int16_t puerto_origen, u_int16_t puerto_destino,double tiempo,double jitter, double delta, double ancho_banda, double perdidas, double mos){

	if(InsertaFinal_ListaH(nodo->lista, clave, destino,puerto_origen, puerto_destino, tiempo, jitter, delta, ancho_banda, perdidas, mos)== NULL){
		printf("Error insertar lista hash\n" );
		return -1;
	}


	return 0;

}




/*actualiza el numero de paquetes recibidos
**Entrada: nodo, numero nuevo
**Salida: 0 si se actualizo, -1 si hay error
*/

double calcularMOS (NodoHash* nodo) {

	double elevado=0.0;
	double mos=0.0;
	double ie_eff;
	double rlq;

	if (nodo == NULL)
		return -1;

	//printf("payload %d\n", nodo->payload);
	if (nodo->payload == 0) {
		ie_eff= 95.0 * nodo->porcentaje_perd / (nodo->porcentaje_perd + 10);
		rlq= 93.2 -ie_eff;
		mos = 1 + rlq*0.035 +  rlq* (100-93.2) * (rlq - 60) * 0.000007;
	}
	else if(nodo->payload == 18) {
		ie_eff= 10 + (95.0 - 10 ) * nodo->porcentaje_perd / (nodo->porcentaje_perd + 17);
		rlq= 93.2 -ie_eff;
		mos = 1 + rlq*0.035 +  rlq* (100-93.2) * (rlq - 60) * 0.000007;
	}
	else{
	elevado= 1.0 + (nodo->porcentaje_perd /2.0);
	//printf("elevado1 %f \t ", elevado);
	elevado = pow( elevado  , 3);
	//printf("elevado2 %f\t ", elevado);
	mos = 1.0 + (4.0 / elevado);
	//printf("mos %f\n", mos);
	}
	nodo->mos =mos;

	return mos;
}

double calcularMOSInt (NodoHash* nodo, double paq_perd) {

	double elevado=0.0;
	double mos=0.0;
	double ie_eff;
	double rlq;

	if (nodo == NULL)
		return -1;

		if (nodo->payload == 0) {
			ie_eff= 95.0 * paq_perd / (paq_perd + 10);
			rlq= 93.2 -ie_eff;
			mos = 1 + rlq*0.035 +  rlq* (100-93.2) * (rlq - 60) * 0.000007;
		}
		else if (nodo->payload == 18) {
			ie_eff= 10 + (95.0 - 10 ) * paq_perd / (paq_perd + 17);
			rlq= 93.2 -ie_eff;
			mos = 1 + rlq*0.035 +  rlq* (100-93.2) * (rlq - 60) * 0.000007;
		}
		else{
			elevado= 1.0 + (paq_perd /2.0);
			//printf("elevado1 %f \t ", elevado);
			elevado = pow( elevado  , 3);
			//printf("elevado2 %f\t ", elevado);
			mos = 1.0 + (4.0 / elevado);
			//printf("mos %f\n", mos);

		}


	nodo->mos =mos;

	return mos;
}

double getMOS(NodoHash* nodo) {
	if (nodo == NULL)
		return -1;
	return nodo->mos;

}



int setNumRecibidos(NodoHash* nodo, int num) {

	if (nodo == NULL)
		return -1;

	nodo->paq_rec = nodo->paq_rec + num;
	return 0;
}

int setRecibidosInt(NodoHash* nodo, int num) {

	if (nodo == NULL)
		return -1;

	nodo->paq_rec_intervalo = nodo->paq_rec_intervalo + num;
	return 0;
}

int setPerdidosInt(NodoHash* nodo, int num) {

	if (nodo == NULL)
		return -1;

	nodo->paq_perd_intervalo = nodo->paq_perd_intervalo + num;
	return 0;
}

int setPorcentajePerdidas(NodoHash* nodo) {

	double porcentaje=0.0;
	if (nodo == NULL)
		return -1;

	porcentaje= (double)nodo->paq_perd/ ((double)nodo->paq_rec + (double)nodo->paq_perd);

	nodo->porcentaje_perd = porcentaje*100.0;

	return 0;
}


double setPorcentajePerdidasInt(NodoHash* nodo, int paq_rec, int paq_perd) {

	double porcentaje=0.0;

	if (nodo == NULL)
		return -1;

	if (paq_perd == 0)
		return 0.0;

	porcentaje= (double)paq_perd/ ((double)paq_rec + (double)paq_perd);

	porcentaje = porcentaje*100.0;

	return porcentaje;
}

double getPorcentajePerdidas(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->porcentaje_perd;
}


int getRecibidosIntervalo(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->paq_rec_intervalo;
}

int getPerdidosIntervalo(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->paq_perd_intervalo;
}

/*devuelve el numero de paquetes perdidos
**Entrada: nodo
**Salida: numero de paquetes perdidos
*/

int getNumPerdidos(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->paq_perd;
}

/*actualiza el numero de paquetes perdidos
**Entrada: nodo, numero nuevo
**Salida: 0 si se actualizo, -1 si hubo algun -1
*/

int setNumPerdidos(NodoHash* nodo, int num) {

	if (nodo == NULL)
		return -1;

	nodo->paq_perd = nodo->paq_perd + num;
	return 0;
}

/*devuelve el tiempo del utimo intervalo
**Entrada: nodo
**Salida: retardo
*/

double getRetardo(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->retardo;
}


double getTiempoInicio(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->tini;
}

double getTiempoFinal(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->tfin;
}

double getIntervalo(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->intervalo;
}
/*devuelve el jitter
**Entrada: nodo
**Salida: jitter
*/
double getJitter(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->jitter;
}


double getJitterIntervalo(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->jitter_intervalo;
}


double getDeltaIntervalo(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->delta_intervalo;
}


double getRecIntervalo(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->paq_rec_intervalo;
}


/*devuelve delta
**Entrada: nodo
**Salida: delta
*/
double getDelta(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->delta;
}

/*devuelve el jitter maximo
**Entrada: nodo
**Salida: jitter maximo
*/

double getMaxJitter(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->maxjitter;
}

/*devuelve el bytes de paquete anterior
**Entrada: nodo
**Salida: bytes
*/
int getBytesAnt(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->bytes_ant;
}

int getBytesIntervalo(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->bytes_intervalo;
}

/*devuelve delta maxima
**Entrada: nodo
**Salida: delta maxima
*/
double getMaxDelta(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

	return nodo->maxdelta;
}
/*actualiza el tiempo del ultimo intervalo
**Entrada: nodo, tiempo
**Salida: 0 si se actualizo, -1 si hubo algun -1
*/
int setRetardo(NodoHash* nodo, double ret) {

	if (nodo == NULL)
		return -1;

	nodo->retardo= nodo->retardo+ ret;

	return 0;
}

int setBytesAnt(NodoHash* nodo, int bytes) {

	if (nodo == NULL)
		return -1;

	nodo->bytes_ant = bytes;

	return 0;
}

int setBytesIntervalo(NodoHash* nodo, int bytes) {

	if (nodo == NULL)
		return -1;

	nodo->bytes_intervalo = nodo->bytes_intervalo + bytes;

	return 0;
}

int setTiempoFin (NodoHash* nodo, double tiempo) {

	if (nodo == NULL)
		return -1;

	nodo->tfin = tiempo;

	return 0;
}

int setIntervalo(NodoHash* nodo, double intervalo) {

	if (nodo == NULL)
		return -1;

	nodo->intervalo = intervalo;

	return 0;
}

int setJitter(NodoHash* nodo, double jitter) {

	if (nodo == NULL)
		return -1;

	nodo->jitter = nodo->jitter + jitter;

	return 0;
}

int setJitterIntervalo(NodoHash* nodo, double jitter) {

	if (nodo == NULL)
		return -1;

	nodo->jitter_intervalo = nodo->jitter_intervalo + jitter;

	return 0;
}

int setDeltaIntervalo(NodoHash* nodo, double delta) {

	if (nodo == NULL)
		return -1;

	nodo->delta_intervalo = nodo->delta_intervalo + delta;

	return 0;
}


int setDelta(NodoHash* nodo, double delta) {

	if (nodo == NULL)
		return -1;

	nodo->delta = nodo->delta + delta;

	return 0;
}

int setMaxJitter(NodoHash* nodo, double jitter) {

	if (nodo == NULL)
		return -1;

	if (nodo->maxjitter < jitter)
		nodo->maxjitter = jitter;

	return 0;
}

int setMaxDelta(NodoHash* nodo, double delta) {

	if (nodo == NULL)
		return -1;

	if (nodo->maxdelta < delta)
		nodo->maxdelta = delta;

	return 0;
}




/*devuelve el tiempo de llegada del ultimo paquete
**Entrada: nodo
**Salida: tiempo
*/
double getLlegadaAnterior(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

  return nodo->retardoAnt;
}

u_int32_t getTimeAnterior(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

  return nodo->timeAnterior;
}

double getJitterAnt(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

  return nodo->jitterant;
}
/*actualiza el tiempo de llegada del ultimo paquete
**Entrada: nodo, tiempo
**Salida: 0 si se actualizo, -1 si hubo algun -1
*/

int setLlegadaAnterior(NodoHash* nodo, double ret) {

	if (nodo == NULL)
		return -1;

    nodo->retardoAnt = ret;
	return 0;
}

int setTimeAnterior(NodoHash* nodo, u_int32_t timestamp) {

	if (nodo == NULL)
		return -1;

    nodo->timeAnterior = timestamp;
	return 0;
}

int setJitterAnt(NodoHash* nodo, double jitter) {

	if (nodo == NULL)
		return -1;

    nodo->jitterant= jitter;
	return 0;
}

int getNumBytes(NodoHash* nodo) {

	if (nodo == NULL)
		return -1;

    return nodo->bytes;
}

int setNumBytes(NodoHash* nodo, int num) {

	if (nodo == NULL)
		return -1;

    nodo->bytes = nodo->bytes + num;
    return 0;
}


double mediaJitter(NodoHash* nodo) {

	double media=0.0;

	if (nodo == NULL)
		return 0.0;

  media= nodo->jitter / (nodo->paq_rec);
	nodo->media_jitter = media;
  return media;
}


double mediaJitterIntervalo(NodoHash* nodo, double jitter, int recibidos) {

	double media=0.0;

	if (nodo == NULL || jitter ==0 || recibidos== 0)
		return 0.0;

  media= jitter / recibidos;

  return media;
}

double mediaDeltaIntervalo(NodoHash* nodo, double delta, int recibidos) {

	double media=0.0;

	if (nodo == NULL || delta ==0 || recibidos== 0)
		return 0.0;

  media= delta / recibidos;

  return media;
}

int setIntervaloNull(NodoHash* nodo){

	nodo->paq_rec_intervalo= 0;
	nodo->paq_perd_intervalo= 0;
	nodo->jitter_intervalo=0.0;
	nodo->delta_intervalo=0.0;
	nodo->bytes_intervalo=0.0;

	return 0;
}

double mediaDelta(NodoHash* nodo) {

	double media=0.0;

	if (nodo == NULL)
		return 0.0;

  media= nodo->delta/ (nodo->paq_rec);
	nodo->media_delta= media;
  return media;
}


double mediaAnchoBanda(NodoHash* nodo) {

	double media=0.0;

	if (nodo == NULL)
		return 0.0;

  media= (nodo->bytes * 8) / (nodo->tfin- nodo->tini);
	nodo->media_ancho_banda=media;

  return media;
}


double mediaAnchoBandaIntervalo(NodoHash* nodo, int bytes, double tini, double tfin) {

	double media=0.0;

	if (nodo == NULL)
		return 0.0;

  media= (bytes * 8) / (tfin - tini);

  return media;
}

double mediaBytes(NodoHash* nodo) {

	double media=0.0;

	if (nodo == NULL)
		return 0.0;

  media= nodo->bytes/ (nodo->paq_rec);
	nodo->media_bytes=media;
  return media;
}



NodoHash* getSiguiente(NodoHash *nodo) {

	if (nodo == NULL)
		return NULL;

	return nodo->siguiente;
}

void printTablaHash(TablaHash *tabla) {

	NodoHash* aux = NULL;
	int i = 0;

	if(tabla == NULL)
		return;

	for(i = 0; i < tabla->tam; i++) {

		if(tabla->nodos[i] != NULL){

			aux = tabla->nodos[i];
			while(aux != NULL){



				fprintf(stdout, "****************\n\n");
				fprintf(stdout, "IP origen: %s\n", aux->origen);
				fprintf(stdout, "IP destino:%s\n", aux->destino);

				fprintf(stdout, "Puerto origen: %d\n", aux->sport);
				fprintf(stdout, "Puerto destino: %d\n", aux->dport);
				fprintf(stdout, "SSRC: %u\n", aux->ssrc);
				fprintf(stdout, "Payload: %d\n", aux->payload);
				fprintf(stdout, "Paquetes recibidos: %d\n", aux->paq_rec);
				fprintf(stdout, "Paquetes perdidos: %d\n", aux->paq_perd);
				fprintf(stdout, "Retardo total: %f\n", aux->retardo);
				fprintf(stdout, "Bytes totales: %d\n", aux->bytes);
				fprintf(stdout, "Max jitter: %f\n", aux->maxjitter);
				fprintf(stdout, "Max delta: %f\n", aux->maxdelta);
				fprintf(stdout, "Mean jitter: %f\n", mediaJitter(aux));
				fprintf(stdout, "Mean delta: %f\n", mediaDelta(aux));
				fprintf(stdout, "Tiempo inicio: %f\n", aux->tini);
				fprintf(stdout, "Tiempo final: %f\n", aux->tfin);
				fprintf(stdout, "Ancho de banda medio: %f\n", mediaAnchoBanda(aux));
				fprintf(stdout, "Bytes media: %f\n", mediaBytes(aux));
				fprintf(stdout, "MOS: %f\n", calcularMOS(aux));
				fprintf(stdout, "Porcentaje de paquetes perdidos: %f\n", aux->porcentaje_perd);

				fprintf(stdout, "\n\n\n");

				aux = aux->siguiente;
			}
		}

	}

	return;

}


void printListaHash(TablaHash *tabla) {

	NodoHash* aux = NULL;
	int i = 0;

	if(tabla == NULL)
		return;

	for(i = 0; i < tabla->tam; i++) {

		if(tabla->nodos[i] != NULL){

			aux = tabla->nodos[i];
			while(aux != NULL){

				imprime_ListaH(aux->lista);
				aux = aux->siguiente;
			}
		}
	}
}



void exportarTablaHash(TablaHash *tabla) {

	FILE * fp;
	NodoHash* aux = NULL;
	int i = 0;

	if(tabla == NULL)
		return;

	fp = fopen ("file.txt", "w+");
	fprintf(fp, "source address |Source Port |Destination Address| Destination Port | SSRC | Payload | Paquetes | Paquetes perdidos| Max jitter | mean Jitter | max delta|mean delta| Tiempo inicio | tiempo final| ancho banda| bytes | MOS \n\n");
	for(i = 0; i < tabla->tam; i++) {

		if(tabla->nodos[i] != NULL){

			aux = tabla->nodos[i];
			while(aux != NULL){


				fprintf(fp, "%s\t %d\t %s\t %d\t %u\t %d\t %d\t %d \t %f\t %f \t %f\t %f\t %f\t %f \t %f\t %f\t %f \n",aux->origen, aux->sport, aux->destino, aux->dport, aux->ssrc, aux->payload, aux->paq_rec, aux->paq_perd, aux->maxjitter, mediaJitter(aux), aux->maxdelta,mediaDelta(aux), aux->tini, aux->tfin,mediaAnchoBanda(aux), mediaBytes(aux), calcularMOS(aux));


				aux = aux->siguiente;
			}
		}

	}
	fclose(fp);
	return;

}
