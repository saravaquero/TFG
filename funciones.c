	#include "funciones.h"


	static void dump_addresses (pcap_addr_t *addresses)
	{
	 pcap_addr_t *addr = addresses;
	 //printf("(");
	 while (addr) {
	  struct sockaddr_in *ip = (struct sockaddr_in *)addr->addr;
	  struct sockaddr_in *nm = (struct sockaddr_in *)addr->netmask;
	  if (ip && nm)
	   //printf("%s/%s ", inet_ntoa(ip->sin_addr), inet_ntoa(nm->sin_addr));
	  addr = addr->next;
	 }
	 //printf(")");
	}

	static void devs_dump (pcap_if_t *devs)
	{
	 pcap_if_t *dev = devs;
	 while (dev) {
	  //printf("dev: %s - %s - ",
	   // dev->name, dev->description);
	  dump_addresses(dev->addresses);
	  //printf("\n");
	  dev = dev->next;
	 }
	}

	char * abrir_dev() {

	    char *dev; /*"dev" contiene el nombre de la interfaz que rastrearemos */
	    bpf_u_int32 mask;
			bpf_u_int32 ip;
	    char errbuff[PCAP_ERRBUF_SIZE]; /* mensaje de error*/
			pcap_if_t *alldevs;
		  pcap_if_t *d;
			int i=0;

			if (pcap_findalldevs(&alldevs, errbuff) == -1) {
		    printf("Error in pcap_findalldevs: %s\n", errbuff);
		    return NULL;
		  }
		  /* Print the list */
		  /*for(d=alldevs; d; d=d->next)
		    {
		        //printf("%d. %s", ++i, d->name);
		        if (d->description)
		            printf(" (%s)\n", d->description);
		        else
		            printf(" (No description available)\n");
		    }*/

		  dev=alldevs->name;


	    /* Find the properties for the device */
			if (pcap_lookupnet(dev, &ip, &mask, errbuff) == -1){
		      fprintf(stderr, "Couldn't get netmask for device %s\n", dev);
		      ip = 0;
		      mask = 0;
		      return NULL;
		  }

			pcap_freealldevs(alldevs);

	    return dev;
	}


	pcap_t * abrir_interfaz_online(char* dev) {

	    char errbuff[PCAP_ERRBUF_SIZE];
	    pcap_t *handle = NULL;

	    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuff);
	    if(handle == NULL) {
	        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuff);
	        return NULL;
	    }

	    return handle;
	}

	pcap_t * abrir_interfaz_offline(char* filename) {

	    char errbuff[PCAP_ERRBUF_SIZE];
	    pcap_t *handle = NULL;

	    handle = pcap_open_offline(filename, errbuff);
	    if(handle == NULL) {
	        fprintf(stderr, "Couldn't open device %s\n", errbuff);
	        return NULL;
	    }

	    return handle;
	}

	double valorAbsoluto(double numero)
	{
    if (numero < 0)
    {
        return numero * -1;
    }
    return numero;
	}

	void analizar_trafico(unsigned char *argument, const struct pcap_pkthdr * header, const u_char *packet) {


			const struct sniff_ethernet *ethernet; /* The ethernet header */
			const struct sniff_ip *ip;         /* Cabecera IP */
			const struct sniff_udp *udp;
			const u_char *ip_header;
			u_int size_ip;
			u_int size_udp;
			u_char protocolo;

			/* Primero comprobamos que sea un paquete IP */
			ethernet =  (struct sniff_ethernet*)(packet);
			if (ntohs(ethernet->ether_type) == ETHERTYPE_IP) {
				/* Igualamos la estructura con el inicio de la cabecera IP y obtenemos
				 * la direccion IP origen y destino*/
				ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
				ip_header = packet + SIZE_ETHERNET;
				/*comprobamos si es udp(campo prtocolo=17 )0x11 IPPROTO_UDP*/
				protocolo = (u_char)*(ip_header + 9);
				if (protocolo == 17) {
					size_ip= IP_HL(ip)*4;

					udp = (struct sniff_udp*)(packet + SIZE_ETHERNET + size_ip);
					size_udp = sizeof(uint32_t)*2;
					rtp = (struct sniff_rtp*)(packet + SIZE_ETHERNET + size_ip + size_udp);
					/*si los dos primeros numeros del playload de udp no son 80 no es rtp*/
					/*udp && udp[0:2] > 1024 && udp[2:2] > 1024 && udp[8]&0xC0 == 0x80*/
					if (RTP_V(rtp) == 2) {
						//PT 72-76	Reserved for RTCP conflict avoidance
						if(RTP_PT(rtp) >= 72 && RTP_PT(rtp) <= 76)
								printf("No es un paquete RTP, es RTCP. Saltando al siguiente...\n");
						else
							printf("es un paquete RTP. Saltando al siguiente...\n");
								//mal: saca algunos udp que empiezan tambien por 80
							return;
					}

				}

			}

			printf("No es un paquete RTP. Saltando al siguiente...\n");
	}



	void paquetes_perdidas(double porcentaje, int tam) {

	  FILE * fp;
	  int num=0;
	  int random;
	  int i;

	  num=tam* (porcentaje/100);

	  fp = fopen ("eliminados.txt", "w+");

	  for (i = 0; i < num; i++) {
	    random = rand() % (tam+1);
	    fprintf(fp, "%d ", random);

	  }

	  fclose(fp);
	  return;
	}

	void obtener_rtp(const struct pcap_pkthdr * header, const u_char *packet, TablaHash* tabla, ListaRTP * listartp){

		const struct sniff_ethernet *ethernet; /* The ethernet header */
		const struct sniff_ip *ip;         /* Cabecera IP */
		const struct sniff_udp *udp;
		const struct sniff_rtp *rtp;
		const struct sniff_rtcp *rtcp;
		const u_char *ip_header;
		const u_char *rtp_header;
		u_int size_ip;
		u_int size_udp;
		double ret = 0.0;
		double retAnterior = 0.0;
		double interArrival = 0.0;
		double interarrival2 = 0.0;
		double deltaTotal = 0.0;
		double retardo;
		double retardo2;
		char ip_dst[16];
		char ip_src[16];
		uint8_t protocolo;
		int payloadType;
		uint16_t size=0;
		uint16_t numSeq;
		uint32_t timestamp;
		uint32_t timeAnterior;
		uint32_t ssrc;
		uint16_t source_port;	/* source port */
		uint16_t dst_port;	/* destination port */
		uint64_t NTP_time;
		uint32_t rtp_time;
		int diferencia;
		double diferencia_tiempo;
		double frecuencia;
		double jitter;
		double jitterant;
		int bytes;
		int bytes_ant;
		double ancho_banda;
		double media_jitter=0.0;
		double media_delta=0.0;
		double media_ancho=0.0;
		double perdidas_int=0.0;
		int num_perd=0;
		double mos_int=0.0;

		NodoHash* nodo = NULL;
		Nodo * nodoC= NULL;



		ethernet =  (struct sniff_ethernet*)(packet);
		if (ntohs(ethernet->ether_type) != ETHERTYPE_IP) {
				//printf("No es un paquete IP. Saltando al siguiente...\n\n");
				return;
		}
		/* Nos colocamos al principio de la cabecera IP */
		ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
		ip_header = packet + SIZE_ETHERNET;
		strcpy(ip_dst, inet_ntoa(ip->ip_dst)); //ntohl
		strcpy(ip_src, inet_ntoa(ip->ip_src));
		//printf("origen %s ,destino %s \n", ip_src, ip_dst);

		/*comprobamos si es udp(campo prtocolo=17 ) IPPROTO_UDP*/
		//protocolo = (uint8_t)*(ip_header + 9);
		protocolo=ip->ip_p;
		//printf("%d\n",protocolo );
		if (protocolo != 0x11) {
				//printf("No es un paquete UDP. Saltando al siguiente...\n\n");
				return;
		}
		/* Obtenemos el campo IHL para averiguar el tamanio de la cabecera IP */
		size_ip= IP_HL(ip)*4;

		udp = (struct sniff_udp*)(packet + SIZE_ETHERNET + size_ip);
		source_port=ntohs(udp->udp_sport);
		dst_port=ntohs(udp->udp_dport);
		//printf("src %u  dst %u \n",source_port, dst_port);
		size_udp = sizeof(uint32_t)*2;

		/*Cabecera RTP*/
		rtp = (struct sniff_rtp*)(packet + SIZE_ETHERNET + size_ip + size_udp);
		rtp_header = packet + SIZE_ETHERNET + size_ip + size_udp;
		if (RTP_V(rtp) != 2) {
				//printf("No es un paquete RTP. Saltando al siguiente...\n\n");
				return;
		}

		if(RTP_PT(rtp) >= 72 && RTP_PT(rtp) <= 76){
			//printf("No es un paquete RTP, es RTCP. Saltando al siguiente...\n");
			return;
		}

		if(RTP_PT(rtp) == 0 || (RTP_PT(rtp) >= 3 && RTP_PT(rtp) <= 5) || (RTP_PT(rtp) >= 7 && RTP_PT(rtp) <= 9) || RTP_PT(rtp) == 12 || RTP_PT(rtp) == 13 || RTP_PT(rtp) == 15 || RTP_PT(rtp) == 18 )
			frecuencia= 8000.0;
		if(RTP_PT(rtp)== 6)
			frecuencia =16000.0;
		if(RTP_PT(rtp)== 10 || RTP_PT(rtp)== 11 )
			frecuencia= 44100.0;
		if(RTP_PT(rtp) >= 25 && RTP_PT(rtp) <= 34)
			frecuencia= 90000.0;


			//printf("%s\n", ip_dst);
			//Microsegundos 1000000L | Mili 1000
			ret = ((header->ts.tv_sec)*1000000L +(header->ts.tv_usec)); // /1millon
			ret=ret/1000.0; //ms

			bytes= header->len;
			//printf("%f\n", ret);
			//numSeq = rtp_header[2] * 256 + rtp_header[2+ 1];
			numSeq=ntohs(rtp->rtp_seqnum);
			ssrc= ntohl(rtp->rtp_ssrc);
			timestamp= ntohl(rtp->rtp_timestamp);
			//payloadType = RTP_PT(rtp);
			//printf("ip_src %s ,ip_dst %s ,numseq %d \n", ip_src, ip_dst, numSeq);

			nodo = buscarNodoHash(tabla, ip_src, ip_dst, source_port, dst_port);
			if(nodo  == NULL){
				insertarNodoHash(tabla, ip_src, ip_dst, NULL, source_port, dst_port, ssrc, RTP_PT(rtp), ret, timestamp, bytes);
				setNumRecibidos(nodo, 1);
				InsertaFinal_ListaRTP(listartp, ip_src, ip_dst,source_port, dst_port, ret, numSeq);
				setIntervalo(nodo, ret/1000); //segundos
				setRecibidosInt(nodo, 1);
				setJitterIntervalo(nodo, jitter);
				setDeltaIntervalo(nodo, interArrival);
				setBytesIntervalo(nodo, bytes);
				setPorcentajePerdidasInt(nodo, 1, 0) ;

				/*if ((insertarNodoHash(tabla, ip_dst, ip_src, ret))== -1){
					printf("error insertando nodo\n");
				}*/
			}
			else{

				setNumRecibidos(nodo, 1); //suma 1 al num de paquetes recibidos


				//calcula el retardo
				retAnterior = getLlegadaAnterior(nodo); //tiempo de llegada del ultimo paquete
				interArrival = ret - retAnterior; //calcula tiempo entre llegadas
				setDelta(nodo,interArrival);
				setMaxDelta(nodo,interArrival);
				//ancho de banda
				bytes_ant=getBytesAnt(nodo);
				ancho_banda= bytes*8/ (ret- retAnterior); //bits/seg
				//printf(" %s , %s ,%d, %f, %f  \n", ip_src, ip_dst, bytes, ret - retAnterior,ancho_banda);
				setLlegadaAnterior(nodo, ret); //actualiza el tiempo de llegada del ultimo paquete
				setBytesAnt(nodo, bytes);
				setNumBytes(nodo,bytes);

				timeAnterior = getTimeAnterior(nodo);
				interarrival2= ((timestamp - timeAnterior) / frecuencia) * 1000;
				//printf("%lf\n", interarrival2);
				setTimeAnterior(nodo, timestamp);

				//printf("%lf   -   %lf \n",interArrival,  interarrival2);
				retardo= interArrival - interarrival2;
				retardo= valorAbsoluto(retardo);
				setRetardo(nodo, retardo);


				//printf("%u   %u  %f\n", timestamp, timeAnterior, interarrival2);
				//printf("%f   %f     %f\n",interArrival, interarrival2, retardo);


				//calcula paquetes perdidos
				nodoC = getNodo(listartp, ip_src, ip_dst, source_port, dst_port);
				setTiempo(nodoC, ret);
				//Comprobación de paquetes perdidos
				if(numSeq - getInfo(nodoC) > 1){
						num_perd= (numSeq - getInfo(nodoC)) -1 ;
						//printf (" %d : %d = %d - %d \n", dst_port, num_perd, numSeq, getInfo(nodoC));
						setNumPerdidos(nodo, num_perd);
				}
				setInfo(nodoC, numSeq);


				//calcula Jitter
				jitterant=getJitterAnt(nodo);
				jitter= jitterant + (retardo - jitterant)/16;
				//printf("ip_src %s ,ip_dst %s ,jiiter %lf\n", ip_src, ip_dst, jitter);
				setMaxJitter(nodo,jitter);
				setJitter(nodo,jitter);
				setJitterAnt(nodo, jitter) ;



				if ((ret/1000 - getIntervalo(nodo)) <= 1.0){

					setRecibidosInt(nodo, 1);
					setPerdidosInt(nodo, num_perd);
					setJitterIntervalo(nodo, jitter);
					setDeltaIntervalo(nodo, interArrival);
					setBytesIntervalo(nodo, bytes);

				}
				else{
					//realizar mediaAnchoBanda
					//poner a 0 elsumatorio
					//printf("Recibidos: %d\n", getRecibidosIntervalo(nodo));
					media_jitter=mediaJitterIntervalo(nodo, getJitterIntervalo(nodo), getRecibidosIntervalo(nodo));
					media_delta=mediaDeltaIntervalo(nodo, getDeltaIntervalo(nodo), getRecibidosIntervalo(nodo));
					media_ancho=mediaAnchoBandaIntervalo(nodo,getBytesIntervalo(nodo), getIntervalo(nodo), ret/1000);
					perdidas_int=setPorcentajePerdidasInt(nodo, getRecibidosIntervalo(nodo), getPerdidosIntervalo(nodo)) ;
					mos_int= calcularMOSInt (nodo, perdidas_int);


					//intervalo_medio= (ret/1000 - getIntervalo(nodo))/2

					if( media_jitter==0.0)
						media_jitter=jitter;
					if( media_delta==0.0)
						media_delta=interArrival;
					if( media_ancho==0.0)
						media_ancho=ancho_banda;
					if( perdidas_int==0.0)
							perdidas_int=0.0;




					insertarLista(nodo, ip_src, ip_dst, source_port, dst_port, ret/1000 ,media_jitter, media_delta, media_ancho, perdidas_int, mos_int);

					setIntervaloNull(nodo);
					setIntervalo(nodo, ret/1000);
				}



				calcularMOS(nodo);
				setPorcentajePerdidas(nodo);
				setTiempoFin(nodo, ret/1000);


				//printf(" %s , %s , %f \n", ip_src, ip_dst, jitter);
			}

			//printf("%s\n", ip_dst);
			//printf("%s\n", ip_src);
			//printf("ip_src %s ,ip_dst %s ,numseq %d \n", ip_src, ip_dst, numSeq);


			//printf("%d   %u  %u \n",numSeq, timestamp, ssrc);


		return;
	}
