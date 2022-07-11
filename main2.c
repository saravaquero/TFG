#include <stdio.h>
#include <time.h>
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>



void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header) {
    printf("Packet capture length: %d\n", packet_header.caplen);
    printf("Packet total length %d\n", packet_header.len);
}

void my_packet_handler(
    u_char *args,
    const struct pcap_pkthdr *packet_header,
    const u_char *packet_body
)
{
    print_packet_info(packet_body, *packet_header);
    return;
}



static void dump_addresses (pcap_addr_t *addresses)
{
 pcap_addr_t *addr = addresses;
 printf("(");
 while (addr) {
  struct sockaddr_in *ip = (struct sockaddr_in *)addr->addr;
  struct sockaddr_in *nm = (struct sockaddr_in *)addr->netmask;
  if (ip && nm)
   printf("%s/%s ", inet_ntoa(ip->sin_addr), inet_ntoa(nm->sin_addr));
  addr = addr->next;
 }
 printf(")");
}

static void devs_dump (pcap_if_t *devs)
{
 pcap_if_t *dev = devs;
 while (dev) {
  printf("dev: %s - %s - ",
    dev->name, dev->description);
  dump_addresses(dev->addresses);
  printf("\n");
  dev = dev->next;
 }
}

pcap_if_t * abrir_dev() {

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
          printf("%d. %s", ++i, d->name);
          if (d->description)
              printf(" (%s)\n", d->description);
          else
              printf(" (No description available)\n");
      }


    /* Find the properties for the device */
    /*if (pcap_lookupnet(dev, &ip, &mask, errbuff) == -1){
        fprintf(stderr, "Couldn't get netmask for device %s\n", dev);
        ip = 0;
        mask = 0;
        return NULL;
    }*/

    pcap_freealldevs(alldevs);

    return alldevs;
}

int main(int argc, char *argv[]) {
    char *device;
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    const u_char *packet;
    struct pcap_pkthdr packet_header;
    int packet_count_limit = 1;
    int timeout_limit = 10000; /* In milliseconds */
    pcap_if_t *alldevs;

    alldevs= abrir_dev();
    device= alldevs->name;

    /* Open device for live capture */
    handle = pcap_open_live(
            "ens33",//device
            BUFSIZ,
            packet_count_limit,
            timeout_limit,
            error_buffer
        );

        if (handle == NULL) {
            fprintf(stderr, "Could not open device %s: %s\n", device, error_buffer);
            return 2;
        }

       pcap_loop(handle, 0, my_packet_handler, NULL);

    return 0;
}
