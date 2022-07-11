#include "consSQL.h"
#include <mysql/mysql.h>


MYSQL* conectar_basedatos(){
  MYSQL *conn= NULL;
  conn =  mysql_init(NULL);
  if (!mysql_real_connect(conn, HOST, USUARIO, PASSWORD, BD, 0, NULL, 0))
	{ /* definir los parámetros de la conexión antes establecidos */
		fprintf(stderr, "%s\n", mysql_error(conn)); /* si hay un error definir cual fue dicho error */
		return NULL;
	}
  return conn;
}

int enviar_consulta(MYSQL *conn){
  if (mysql_query(conn, "select * from datos"))
	{ /* definicion de la consulta y el origen de la conexion */
		fprintf(stderr, "%s\n", mysql_error(conn));
		return -1;
	}
  return 0;
}


/* SELECT tini, tfin, ancho_banda/(tfin-tini)
FROM RTP;
WITH y AS
(
  SELECT DISTINCT tini, SUM(ancho_banda/(tfin-tini)) OVER (ORDER BY tini) as ancho_banda1, tfin, SUM(ancho_banda/(tfin-tini)) OVER (ORDER BY tfin) as ancho_banda2
  FROM RTP
)
SELECT d, coalesce(cumsum_upper, 0),  coalesce(cumsum_lower, 0), coalesce(cumsum_upper, 0) - coalesce(cumsum_lower, 0) AS timeserie FROM
(
SELECT g.d,
  (SELECT ancho_banda1
   FROM y
   WHERE y.tini <= g.d
   ORDER BY tini DESC
   LIMIT 1) as cumsum_upper,
   (SELECT ancho_banda
   FROM y
   WHERE y.tfin <= g.d
   ORDER BY tfin DESC
   LIMIT 1) as cumsum_lower
FROM
  (SELECT generate_series(min(tini), max(tfin), '1 day')::date as d
   FROM y) as g
ORDER BY d ASC
) as res*/

int crear_tablas(MYSQL *conn){

  int db;

  if(conn == NULL) {
       fprintf(stderr, "%s\n", mysql_error(conn));
       return -1;
   }

  db = mysql_query(conn, DROP_TABLE);
     if (db) {
         fprintf(stderr, "%s\n", mysql_error(conn));
         return -1;
     }

  db = mysql_query(conn, CREATE_TABLE_RTP);
  if (db){
    fprintf(stderr, "%s\n", mysql_error(conn));
    return -1;
  }

  db = mysql_query(conn, CREATE_TABLE_LISTAHASH);
  if (db){
    fprintf(stderr, "%s\n", mysql_error(conn));
    return -1;
  }


  return 0;
}



void insertar_datos_tabla_rtp(MYSQL *conn, TablaHash* tabla){

  int i=0, j=0;
  char consulta[10000];
  int db=0;

  char *origen;   	/*IP origen */
	char *destino;		/*IP destino */
	int dport;	/*puerto UDP destino*/
	int sport;	/*puerto UDP origen*/
	int payload;
	u_int32_t ssrc;
	int paq_rec;					/*Paquetes Recibidos */
	int paq_perd;					/*Paquetes Perdidos */
	double retardo;
	double retardoAnt;
	int bytes;
	int bytes_ant;
	NodoHash *siguiente;
	double jitter;
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
  double porcentaje_perd;
  double mos;


  NodoHash* aux=NULL;


  MYSQL_RES *res; /* variable que contendra el resultado de la consuta */
  MYSQL_ROW row;/* variable que contendra los campos por cada registro consultado */


  if(tabla==NULL )
    return;


  for(i=0; i<tabla->tam;i++){

    if (tabla->nodos[i] != NULL){
      //printf("%d",i);

      aux= tabla->nodos[i];
      while(aux!= NULL){

        origen= getOrigen(aux);
      	destino= getDestino(aux);
      	dport= getPuertoDestino(aux);	/*puerto UDP destino*/
      	sport= getPuertoOrigen(aux);/*puerto UDP origen*/
      	payload= getPayload(aux);
      	ssrc= getSSRC(aux);
      	paq_rec= getNumRecibidos(aux);			/*Paquetes Recibidos */
      	paq_perd=getNumPerdidos(aux);		/*Paquetes Perdidos */
      	maxjitter=getMaxJitter(aux);
      	maxdelta=getMaxDelta(aux);
      	tini=getTiempoInicio(aux);
      	tfin=getTiempoFinal(aux);
      	media_ancho_banda= mediaAnchoBanda(aux);
      	media_delta= mediaDelta(aux);
      	media_jitter= mediaJitter(aux);
      	media_bytes=mediaBytes(aux);
        porcentaje_perd=getPorcentajePerdidas(aux);
        mos=calcularMOS(aux);

        j=j+1;
        sprintf(consulta, "INSERT INTO RTP(id, origen,destino,dport,sport, payload,ssrc, paq_rec,paq_perd, maxjitter,maxdelta,media_ancho_banda,media_delta ,media_jitter ,media_bytes, tini ,tfin, porcentaje_perd, MOS) VALUES(%d, '%s', '%s', %d, %d, %d, %d, %d, %d, %f, %f, %f, %f, %f, %f,FROM_UNIXTIME(%f), FROM_UNIXTIME(%f), %f, %f)",j,  origen,destino,dport,sport, payload,ssrc, paq_rec, paq_perd, maxjitter,maxdelta,media_ancho_banda,media_delta ,media_jitter ,media_bytes, tini, tfin, porcentaje_perd, mos);

        db= mysql_query(conn, consulta);
        if(db != 0){
          fprintf(stderr, "%s\n", mysql_error(conn));
          return;
        }
        /*
        res=mysql_store_result(conn);
        if(!res){
          fprintf(stderr, "%s\n", mysql_error(conn));
          return;
        }

        memset(consulta, 0, sizeof(consulta));
        */
        //mysql_free_result(res);

        aux = getSiguiente(aux) ;


      }
    }
  }
return;
}






void insertar_datos_tabla_listahash(MYSQL *conn, TablaHash* tabla){

  char *origen;   	/*IP origen */
	char *destino;		/*IP destino */
	int dport;	/*puerto UDP destino*/
	int sport;	/*puerto UDP origen*/
  int i;
  int j=0;
  double jitter;
	double tiempo;
	double delta;
  double ancho_banda;
  double perdidas;
  double mos;

  char consulta[10000];
  int db=0;
  double tiempoant=0.0;


  NodoHash* aux=NULL;
  ListaH * lista=NULL;
  NodoH * pn=NULL;

  MYSQL_RES *res; /* variable que contendra el resultado de la consuta */
  MYSQL_ROW row;/* variable que contendra los campos por cada registro consultado */


  if(tabla==NULL )
    return;


    for(i=0; i<tabla->tam;i++){

      if (tabla->nodos[i] != NULL){
        //printf("%d",i);

        aux= tabla->nodos[i];
        while(aux!= NULL){

          lista= getLista(aux);

          if(!lista)
              return;


          pn=lista->nodo;
          while(pn!=NULL){

            origen= getHclave(pn) ;
          	destino= getHDestino(pn);
          	dport=getH_dport(pn);
          	sport=getH_sport(pn);;

            jitter=getHJitter(pn);
          	tiempo= getHTiempo(pn);
          	delta=getHDelta(pn);
            ancho_banda=getHAnchoBanda(pn);
            perdidas=getHPerdidas(pn);
            mos=getHMOS(pn);

            //if (tiempo - tiempoant >= 2.0){
              j=j+1;
              sprintf(consulta, "INSERT INTO listahash(id, origen,destino,dport,sport, jitter, delta, tiempo, ancho_banda, perdidas, MOS) VALUES(%d, '%s', '%s', %d, %d, %f, %f,FROM_UNIXTIME(%f), %f, %f, %f)",j,  origen,destino,dport,sport, jitter, delta,tiempo, ancho_banda, perdidas, mos);

              db= mysql_query(conn, consulta);
              if(db != 0){
                fprintf(stderr, "%s\n", mysql_error(conn));
                return;

              }

              tiempoant=tiempo;

            //}

            pn=pn->siguiente;


          }
          tiempoant=0;
          aux = getSiguiente(aux) ;
        }
      }
    }







  return;
}
