#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pcap.h>
#include "hash.h"
#include "consSQL.h"
#include "listaRTP.h"
#include "listaHash.h"

#define ETHER_ADDR_LEN	6
#define SIZE_ETHERNET 14
#define LEN_UDP 8
#define ETHERTYPE_IP 0x0800
#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)

#define RTP_V(rtp)		(((rtp)->rtp_version) >> 6)
#define RTP_P(rtp)		((((rtp)->rtp_version) & 0x20) >> 5)
#define RTP_X(rtp)		((((rtp)->rtp_version) & 0x10) >> 4)
#define RTP_CC(rtp)		(((rtp)->rtp_version) & 0x0f)
#define RTP_M(rtp)		(((rtp)->rtp_payloadType) >> 7)
#define RTP_PT(rtp)		(((rtp)->rtp_payloadType) &0x7F)

#define RTCP_V(rtcp)		(((rtcp)->rtcp_version) >> 6)
#define RTCP_P(rtcp)		((((rtcp)->rtcp_version)& 0x20) >> 5)
#define RTCP_PT(rtcp)		(((rtcp)->rtp_version) & 0x1f)

/* Cabecera Ethernet*/
	struct sniff_ethernet {
		uint8_t ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
		uint8_t ether_shost[ETHER_ADDR_LEN]; /* Source host address */
		uint16_t ether_type; /* IP? ARP? RARP? etc */
	};


  /* Cabecera IP */
  	struct sniff_ip {
  		uint8_t ip_vhl;		/* version << 4 | header length >> 2 */
  		uint8_t ip_tos;		/* type of service */
  		uint16_t ip_len;		/* total length */
  		uint16_t ip_id;		/* identification */
  		uint16_t ip_off;		/* fragment offset field */
  	#define IP_RF 0x8000		/* reserved fragment flag */
  	#define IP_DF 0x4000		/* don't fragment flag */
  	#define IP_MF 0x2000		/* more fragments flag */
  	#define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
  		uint8_t ip_ttl;		/* time to live */
  		uint8_t ip_p;		/* protocol */ /*uint8_t*/
  		uint16_t ip_sum;		/* checksum *//*uint16_t*/
  		struct in_addr ip_src,ip_dst; /* source and dest address */
  	};


    /* Cabecera UDP */

	struct sniff_udp{
		uint16_t udp_sport;	/* source port */
		uint16_t udp_dport;	/* destination port */
		uint16_t udp_len;
    uint16_t udp_sum;		/* checksum */

	};

	struct sniff_rtp{
		/*byte 0*/
		uint8_t rtp_version; //version,padding,extesion,lencsrc


		/*byte 1*/
		uint8_t rtp_payloadType; //PT, m

		/*byte 2,3*/
		uint16_t rtp_seqnum;

		/*byte 4-7*/
		uint32_t rtp_timestamp;

		/*byte 8-11*/
		uint32_t rtp_ssrc;

		uint32_t rtp_csrc;

	};

	struct sniff_rtcp{
		/*byte 0*/
		uint8_t rtcp_version;

		uint8_t rtcp_pt;

		uint16_t rtcp_length;

		uint32_t rtcp_ssrc_sender;

		uint64_t rtcp_ntp_timestamp;//least significant word
		/*byte 4-7*/
		uint32_t rtcp_rtp_timestamp;

	};


const struct sniff_ethernet *ethernet; /* The ethernet header */
const struct sniff_ip *ip; /* The IP header */
const struct sniff_udp *udp; /* The TCP header */
const struct sniff_rtp *rtp;/* The RTP header */
const struct sniff_rtp *rtcp;/* The RTP header */
const char *payload; /* Packet payload */

u_int size_ip;
u_int size_udp;
/**
/*FUNCIONES**/
static void dump_addresses (pcap_addr_t *addresses);
static void devs_dump (pcap_if_t *devs);
char * abrir_dev();
pcap_t * abrir_interfaz_online(char* dev) ;
pcap_t * abrir_interfaz_offline(char* filename) ;
double valorAbsoluto(double numero);
void analizar_trafico(unsigned char *argument, const struct pcap_pkthdr * header, const u_char *packet);
void obtener_rtp(const struct pcap_pkthdr * header, const u_char *packet, TablaHash* tabla, ListaRTP * listartp);
void paquetes_perdidas(double porcentaje, int tam) ;

#endif
