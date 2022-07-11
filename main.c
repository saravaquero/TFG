#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pcap.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include "funciones.h"


int main(int argc, char* argv[]) {
  pcap_t *handle = NULL;
  char* filename ;
  //char filename[100];
  int num_paquetes=4268;
  int tam=0;
  int x=0;
  double perdidas= 0.5;
  char errbuff[PCAP_ERRBUF_SIZE];
  int * contador=0;
  char *dev; /*"dev" contiene el nombre de la interfaz que rastrearemos */
  pcap_if_t *alldevs;
  pcap_if_t *d;
  bpf_u_int32 mask;
  bpf_u_int32 ip;
  FILE * fp;
  MYSQL *basedatos= NULL;

  TablaHash* tabla = NULL;
  ListaRTP * lista_rtp =NULL;


  struct pcap_pkthdr header;
  const u_char *packet;
  struct pcap_pkthdr * packet_header;

  if (argc<2){
    printf("Debe introducir el nombre del fichero \n");
    exit(1);
  }


  //abrir dev
  dev=abrir_dev();
  //captura offline
  //handle= abrir_interfaz_offline(filename);
  handle= abrir_interfaz_offline(argv[1]);
  /*captura online
  handle=abrir_interfaz_online(dev);*/

  //printf("aqui bien");
	packet = pcap_next(handle, &header);
  /* Print its length */
  //printf("Jacked a packet with length of [%d]\n", header.len);



  //analiza el trafico, que paquetes son rtp y cuales no
  /*if( pcap_loop(handle, -1, analizar_trafico, NULL) < 0 )
  {
    perror("pcap_loop");
  }

  /*calculo numero de paquetes
  while (pcap_next(handle, &header)) {
    num_paquetes++;
  }
  printf("numero de paquetes: %d\n", num_paquetes);
  */



  tabla = crearTablaHash(10001);
    if(tabla == NULL){
      printf("Error al crear tabla hash");
      return -1;
  }

  lista_rtp= crea_ListaRTP();
    if(lista_rtp == NULL){
      printf("Error al crear lista");
      return -1;
  }


  basedatos= conectar_basedatos();
  if (!basedatos) {
      fprintf(stderr, "%s\n", mysql_error(basedatos));
      return-1;
  }


  if(crear_tablas(basedatos) != 0)
    return -1;


  while((x= pcap_next_ex(handle, &packet_header, &packet)) >= 0){
    obtener_rtp(packet_header, packet, tabla, lista_rtp);
    tam=tam+1;
  }
  //printf("TAMAÑO %d\n",tam );
  paquetes_perdidas(perdidas, tam) ;

  printTablaHash(tabla);


  insertar_datos_tabla_rtp(basedatos, tabla);
  insertar_datos_tabla_listahash(basedatos, tabla);



  //printf("%d\n",x);
  //imprime_ListaRTP(lista_rtp);

  //printListaHash(tabla);
  //exportarTablaHash(tabla);

  borra_ListaRTP(lista_rtp);
  borrarTablaHash(tabla);
  mysql_close(basedatos);
  mysql_library_end();
  pcap_close(handle);
  printf("todo bien");
  return 0;

}
