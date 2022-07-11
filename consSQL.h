#ifndef CONSSQL_H
#define CNSSQL_H


#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "listaRTP.h"
#include "listaHash.h"
#include <math.h>

#define HOST "localhost" // servidor local
#define USUARIO "root" //nombre de usuario
#define PASSWORD "tfg" //contraseña
#define BD "bd_rtp"  // base de datos

#define CREATE_TABLE_RTP "CREATE TABLE RTP( id INTEGER PRIMARY KEY, origen CHAR(100) NOT NULL, destino CHAR(100) NOT NULL, dport INTEGER NOT NULL, sport INTEGER NOT NULL, payload INTEGER, ssrc INTEGER, paq_rec INTEGER, paq_perd INTEGER,	maxjitter DOUBLE, maxdelta DOUBLE, tini DATETIME, tfin DATETIME, media_ancho_banda DOUBLE, media_delta DOUBLE, media_jitter DOUBLE, media_bytes DOUBLE, porcentaje_perd DOUBLE, MOS DOUBLE)"
#define DROP_TABLE "DROP TABLE IF EXISTS RTP, listahash"
#define CREATE_TABLE_LISTAHASH " CREATE TABLE listahash( id INTEGER PRIMARY KEY, origen CHAR(100) NOT NULL, destino CHAR(100) NOT NULL, dport INTEGER NOT NULL, sport INTEGER NOT NULL, jitter DOUBLE, delta DOUBLE, tiempo DATETIME, ancho_banda DOUBLE,  perdidas DOUBLE,  MOS DOUBLE)" //, rtp INTEGER, FOREIGN KEY(rtp) REFERENCES RTP(id)

/*
MYSQL *conn; /* variable de conexión para MySQL */
//MYSQL_RES *res; /* variable que contendra el resultado de la consuta */
//MYSQL_ROW row; /* variable que contendra los campos por cada registro consultado */
//char *server = "localhost"; /*direccion del servidor 127.0.0.1, localhost o direccion ip */
//char *user = "root"; /*usuario para consultar la base de datos */
//char *password = "root"; /* contraseña para el usuario en cuestion */
//char *database = "prueba"; /*nombre de la base de datos a consultar */
//conn = mysql_init(NULL); /*inicializacion a nula la conexión */

MYSQL* conectar_basedatos();
int enviar_consulta(MYSQL *conn);
int crear_tablas(MYSQL *conn);
void insertar_datos_tabla_rtp(MYSQL *conn, TablaHash* tabla);
void insertar_datos_tabla_listahash(MYSQL *conn, TablaHash* tabla);

#endif
