#include <stdio.h>
#include <errno.h>
#include <string.h> 
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <netinet/ip.h>


#define SERV_PORT 8081
#define CLI_PORT 8081
#define SERV_IP "127.0.0.1"
#define BUFF_SIZE 64
#define PACK_SIZE 128
void create_udp_header(char *packet, short dport, short sport, short datalen)
{
	struct udphdr *udph;

	udph = (struct udphdr *) packet;
	udph->source = htons(sport);
	udph->dest = htons(dport);
	udph->check = 0;
	udph->len = htons(datalen + sizeof(struct udphdr));
}
int main(void)
{

	char buffer[1024];
			int n, lens; 

	struct sockaddr_in serv, cli;
	socklen_t len;
	int sock;
	int rc;	
	char buff[SERV_PORT] = "Hello server!\0";
	char *packet;
	struct iphdr *iph = NULL;
	struct udphdr *udph = NULL;
	struct in_addr saddr;

	memset(&cli, 0, sizeof(cli));
	sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if (sock < 0) {
		perror("Create socket");
		exit(1);
	}

	len = sizeof(struct sockaddr_in);
	rc = getsockname(sock, (struct sockaddr *) &cli, &len);
	if (rc < 0) {
		perror("Can`t get client end-point");
		exit(1);
	} 

	printf("ip - > %s port-> %d\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));

	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(SERV_PORT);
	serv.sin_addr.s_addr = inet_addr(SERV_IP);

	 packet= calloc(PACK_SIZE, sizeof(char));
	memmove(packet + 8, buff, strlen(buff) + 1);
	create_udp_header(packet, SERV_PORT, CLI_PORT, strlen(buff) + 1);
 
	rc = sendto(sock, packet, PACK_SIZE, 0, (struct sockaddr *)&serv, len);
	if (rc < 0) {
		perror("Can`t send message to server");
	} else {
		printf("Send to server bytes -> %d\n", rc);
	}

	
 
	while(1) {
		
		memset(packet, 0, PACK_SIZE);
		len = sizeof(struct sockaddr_in);
	
		rc = recvfrom(sock, packet, PACK_SIZE, 0, (struct sockaddr *)&serv, &len);
		if (rc < 0) {
			perror("Can`t recv message from server");
		} else {
			iph = (struct iphdr *) packet;
			udph = (struct udphdr *) (packet + sizeof(struct iphdr));
			saddr.s_addr = iph->saddr;
			printf("Header of message ip - %s sport - %d dport - %d\n", inet_ntoa(saddr), 
					ntohs(udph->source), ntohs(udph->dest));
			printf("From ip-> %s port-> %d\nRecv from server message -> %s %d bytes\n", 
				inet_ntoa(serv.sin_addr), ntohs(serv.sin_port), 
				packet + sizeof(struct iphdr) + sizeof(struct udphdr), rc);
		}
	
	}
	close(sock);
	return 0;
}