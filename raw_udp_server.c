//sudo tcpdump -i lo udp port 8081 -XX -vvv
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <netinet/udp.h>
#define MAXBUFLEN 1000
#define PORT 8081
char path_index[100];
#define MAXLINE 1024
// struct udphdr {
// 	u_short	source;		/* source port */
// 	u_short	dest;		/* destination port */
// 	u_short	len;		/* udp length */
// 	u_short	check;			/* udp checksum */
// };

int main(int argc, char *argv[])
{
    struct udphdr *udp;
    int n,len;
    
    char buffer[1024]="kill me plz";
    char bufer[MAXLINE];// recvfrom
    //struct udp_packet p;
    int sockfd;
	struct sockaddr_in servaddr;
	int rv;
	int numbytes;
    char *packet;
    
	//memset(&p,0,sizeof(struct udp_packet));
    memset(&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
    printf("_1_\n");
     packet= calloc(128, sizeof(char));
    udp = (struct udphdr *) packet; ///////////// чё за жопа то ааааааа
    
    
    
	printf("_2_\n");

    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) == -1) 
    {
        perror("talker: socket");
        
    }

    printf("_2_2\n");
  //  udp = (struct udphdr *) packet;
    printf("_2_31\n");
    udp->source = htons(8081);
    printf("_2_32\n");
    udp->dest = htons(8081);
    printf("_2_33\n");
    udp->len = htons(128);
    udp->check = 0;
    memmove(packet + 8, buffer, strlen(buffer) + 1);
    printf("_3_\n");
    len = sizeof(struct sockaddr_in);
	if ((numbytes = sendto(sockfd,packet,200,0,(struct sockaddr *)&servaddr,len)) == -1) 
    {
		perror("talker: sendto");
		exit(1);
	}

    printf("send to --%d\n",numbytes );
    printf("_4_\n");
    
    while(1)
    {
       n = recvfrom(sockfd,(char *)bufer, sizeof(bufer),0, (struct sockaddr *) &servaddr, &len); 
        //bufer[n] = '\0'; 
        printf("client: ");
        for(int d=0; d<n; d++)
        printf("%c",bufer[d]);
         printf("\n");
        printf("Server : |%s| --%d bytes --%d\n", bufer,n,sizeof(buffer)); 
       
    }
	printf("_5_\n");

	
	close(sockfd);

	return 0;
}

