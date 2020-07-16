/*
	IPK 2. projekt - ZETA sniffer paketu
	2020
	autor: Radek Svec (xsvecr01@stud.fit.vutbr.cz)
*/


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <pcap.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <time.h>

#define SIZE_ETHERNET 14

/*
struktura pro IP hlavicku na zjisteni IP adres
zdroj:
https://www.tcpdump.org/pcap.html
This document is Copyright 2002 Tim Carstens. All rights reserved.
*/
/* IP header */
struct sniff_ip {
	u_char ip_vhl;		/* version << 4 | header length >> 2 */
	u_char ip_tos;		/* type of service */
	u_short ip_len;		/* total length */
	u_short ip_id;		/* identification */
	u_short ip_off;		/* fragment offset field */
	#define IP_RF 0x8000		/* reserved fragment flag */
	#define IP_DF 0x4000		/* dont fragment flag */
	#define IP_MF 0x2000		/* more fragments flag */
	#define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
	u_char ip_ttl;		/* time to live */
	u_char ip_p;		/* protocol */
	u_short ip_sum;		/* checksum */
	struct in_addr ip_src,ip_dst; /* source and dest address */
};
#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)                (((ip)->ip_vhl) >> 4)
/*
konec https://www.tcpdump.org/pcap.html
*/

//struktury pro TCP a UDP na zjisteni portu zdroje a cile
struct UDP_hdr {
	u_short	uh_sport;
	u_short	uh_dport;
};
struct TCP_hdr {
	u_short	th_sport;
	u_short	th_dport;
};

// promenne pro nastaveni flagu
int tcp, udp = 0;
int port = 65536;


//prototypy funkci
void callback(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
void print_payload(const u_char *payload, struct pcap_pkthdr header);


/************
	MAIN
************/
/*
	stara se o zpracovani argumentu, vyber rozhrani a otevreni rozhrani pro zachytavani paketu
*/
int main(int argc, char **argv)
{
	char *dev;						//rozhrani
	char errbuf[PCAP_ERRBUF_SIZE];	//buffer pro errory
	pcap_if_t *alldevs, *d;			//rozhrani
	const u_char *packet;			//paket
	int i = 0;						//citac rozhrani
	int opt; 
	int n_packet = 1;				//pocet paketu
	int i_exists = 0;				//kontrolni promenna jestli rozhrani existuje
	char filter_exp[40] = "";		//pole pro filtry


	/*
	PARSING ARGUMENTU
	// */
	/*
	vypis kratke napovedy pokud nejsou zadany argumenty a vypis napovedy pomoci --help
	*/
	if(argc == 1){
		printf("./ipk-sniffer -i rozhraní [-p port] [--tcp|-t] [--udp|-u] [-n num]\n");
		printf("--help pro napovedu.\n");
		return 0;
	}
	if((strcmp(argv[1], "--help") == 0 && argc == 2)){
		printf("Pouziti:\n");
		printf("./ipk-sniffer -i rozhraní [-p port] [--tcp|-t] [--udp|-u] [-n num]\n\n");
		printf("-i eth0 (rozhrani, na kterem se bude poslouchat. Nebude-li tento parametr uveden, vypíše se seznam aktivních rozhraní)\n");
		printf("-p 23 (bude filtrování paketů na daném rozhraní podle portu; nebude-li tento parametr uveden, uvažují se všechny porty)\n");
		printf("-t nebo --tcp (bude zobrazovat pouze tcp pakety)\n");
		printf("-u nebo --udp (bude zobrazovat pouze udp pakety)\n");
		printf("Pokud nebude -tcp ani -udp specifikováno, uvažují se TCP a UDP pakety zároveň\n");
		printf("-n 10 (určuje počet paketů, které se mají zobrazit; pokud není uvedeno, zobrazí se pouze 1 paket)\n");
		return 0;
	}

	/*
	interface neni zadan
	*/
	if(strcmp(argv[1], "-i") == 0 && argc == 2){
		if (pcap_findalldevs(&alldevs, errbuf) == -1){
			fprintf(stderr,"Chyba ve funkci pcap_findalldevs: %s\n", errbuf);
			return 2;
		}
		printf("Dostupna rozhrani:\n");
		for(d=alldevs; d; d=d->next){
			printf("%d. %s\n", ++i, d->name);
		}
		return 0;
	}

	/*
	parse dlouhych argumentu --tcp a --udp
	*/
	int a = 0;
	int is_i = 0;
	while(a < argc){
		if(strcmp(argv[a], "-i") == 0){
			is_i = 1;
		}
		if(strcmp(argv[a], "--tcp") == 0){
			tcp = 1;
			argv[a] = "";
		}
		if(strcmp(argv[a], "--udp") == 0){
			udp = 1;
			argv[a] = "";
		}
		a++;
	}

	if(is_i == 0){
		fprintf(stderr, "Neni zvoleno zadne rozhrani.\n");
		return 1;
	}

	/*
	parse kratkych argumentu
    */
	while((opt = getopt(argc, argv, ":i:p:tun:")) != -1){
		switch(opt){
			case 'i':
				dev = optarg;
				break;
				case 'p':
				if((port = atoi(optarg)) > 65535 || (port = atoi(optarg)) < 0){
					fprintf(stderr, "Chybna hodnota: %s\n", optarg);
					return 1;
				}
				break;
			case 't':
				tcp = 1;
				break;
			case 'u':
				udp = 1;
				break;
			case 'n':
				n_packet = atoi(optarg);
				break;
			case ':':
				fprintf(stderr, "Chybi hodnota parametru: %c\n", optopt);
				return 1;
			case '?':
				fprintf(stderr, "Neznamy parametr: %c\n", optopt);
				return 1;
		}
	}

	

	/*
	vyhledani vsech zarizeni a zvoleni vybraneho
	*/
	if (pcap_findalldevs(&alldevs, errbuf) == -1){
		fprintf(stderr,"Chyba v pcap_findalldevs: %s\n", errbuf);
		return 2;
	}
	for(d=alldevs; d; d=d->next){
		if(strcmp(d->name, dev) == 0){
			i_exists = 1;
			break;
		}
	}
	if(i_exists == 0){
		fprintf(stderr, "Zvolene rozhrani neexistuje.\n");
		return 1;
	}

	
	/*
	nastaveni filtru podle vlozenych argumentu
	*/
	if(tcp == 1 && udp == 0){
		strcat(filter_exp, "tcp ");
	}
	else if(udp == 1 && tcp == 0){
		strcat(filter_exp, "udp ");
	}
	else{
		strcat(filter_exp, "(tcp or udp) ");
	}
	if(port != 65536){
		strcat(filter_exp, "and port ");
		char str[12];
		sprintf(str, "%d ", port);
		strcat(filter_exp, str);
	}

	/*
	otevreni rozhrani a zachytavani packetu
	*/
	/*
	prevzato z https://www.tcpdump.org/pcap.html
	This document is Copyright 2002 Tim Carstens. All rights reserved.
	*/
	pcap_t* handle = pcap_open_live(dev, 65535, 1, 200, errbuf);
	if(!handle){
		printf("Chyba v pcap_open_live(): %s\n",errbuf);
		return 2;
	}

	/*
	nastaveni filtru pro zachytavani
	*/
	struct bpf_program filter;
	if (pcap_compile(handle, &filter, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
		printf("Spatny filtr: %s\n", pcap_geterr(handle));
		return 3;
    }
	if (pcap_setfilter(handle, &filter) == -1) {
		printf("Chyba pri nastavovani filtru: %s\n", pcap_geterr(handle));
		return 3;
    }

	/*
	volani funkce callback pro kazdy packet
	*/
	pcap_loop(handle, n_packet, callback, NULL);
	pcap_close(handle);
	/*
	konec prevzate casti
	*/

	return 0;
}

/*****************
	PRINT_PAYLOAD
*****************/
/*
	funkce vypisuje packet do pozadovaneho tvaru
*/
void print_payload(const u_char *payload, struct pcap_pkthdr header){
	//pomocne ukazatele
	const u_char *ch;
	const u_char *ch_asci;

	int remaining = header.caplen;
	int line = 0;
	int hexline = 0;
	ch = payload;

	while (1){
		ch_asci = ch;
		line = 16 % remaining;

		//zpracovani a tisk jednoho radku, pri prvnim prochazeni radku se tisknou hex hodnoty a pri druhem prochazeni se tisknou ascii znaky
		printf("\n0x%04X  ", hexline);
		for(int i = 0; i < line; i++){
			if(i == 8) printf(" ");
			printf("%02x ", *ch);
			ch++;
			if (i == line-1){
				printf(" ");
				for(int j = 0; j < line; j++){
					if(j == 8) printf(" ");
					if (isprint(*ch_asci))
					printf("%c", *ch_asci);
					else
						printf(".");
					ch_asci++;
				}
			}
		}

		//odecteni jednoho radku od zbyvajciho poctu znaku a pricteni k hex hodnote radku
		remaining -= line;
		hexline += 16;

		//vytisk posledniho radku a vyskoceni z cyklu
		if(remaining <= 16){
			printf("\n0x%04X  ", hexline);
			int charcnt = 7;
			for(int i = 0; i < remaining; i++){
				if(i == 8){
					printf(" ");
					charcnt++;
				}
				printf("%02x ", *ch);
				charcnt += 3;
				ch++;
				if (i == remaining-1){
					//dorovnani poctu mezer do vypisu ascii znaku
					for(charcnt; charcnt < 56; charcnt++) printf(" ");
					
					printf(" ");
					//vypis ascii
					for(int j = 0; j < remaining; j++){
						if(j == 8) printf(" ");
						if (isprint(*ch_asci))
						printf("%c", *ch_asci);
						else
							printf(".");
						ch_asci++;
					}
				}
			}
			break;
		}
	}
	printf("\n\n");
	return;
}

/*****************
	CALLBACK
*****************/
/*
	funkce vykonavajici se operace pro kazdy paket
*/
void callback(u_char *args, const struct pcap_pkthdr *header, const u_char *packet){
	const u_char *ip_header;
	const u_char *payload;
	const struct sniff_ip *ip;

	//vypocet pozice protokolu
	ip_header = packet + SIZE_ETHERNET;
	u_char protocol = *(ip_header + 9);

	ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);

	//prevedeni casu na tisknutelny format
	time_t *timesecs = (time_t*)&header->ts.tv_sec;

	struct tm *time = localtime(timesecs);
	int hr = time->tm_hour;
	int min = time->tm_min;
	int sec = time->tm_sec;
	int usec = header->ts.tv_usec;
	
	//vypis casoveho formatu
	printf("\n%02d:%02d:%02d.%d ",hr, min, sec, usec);

	//kontrola protokolu a rozhodnuti, zda se jedna o TCP nebo UDP paket
	if (protocol == IPPROTO_TCP) {
		struct TCP_hdr *tcp;
		tcp = (struct TCP_hdr*)(packet + SIZE_ETHERNET + IP_HL(ip)*4);

		printf("%s : %d > ", inet_ntoa(ip->ip_src), ntohs(tcp->th_sport));
		printf("%s : %d\n", inet_ntoa(ip->ip_dst), ntohs(tcp->th_dport));
		print_payload(packet, *header);
	}

	else if (protocol == IPPROTO_UDP) {
		struct UDP_hdr *udp;
		udp = (struct UDP_hdr*)(packet + SIZE_ETHERNET + IP_HL(ip)*4);

		printf("%s : %d > ", inet_ntoa(ip->ip_src), ntohs(udp->uh_sport));
		printf("%s : %d\n", inet_ntoa(ip->ip_dst), ntohs(udp->uh_dport));
		print_payload(packet, *header);
	}
	return;
}