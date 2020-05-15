
#include <netinet/udp.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
//#define __USE_BSD
//#define __FAVOR_BSD

unsigned short          /* this function generates header checksums */
csum (unsigned short *buf, int nwords)
{
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return ~sum;
}

int main() {
struct ip *myip;
struct udphdr *myudp;
struct sockaddr_in sin;
char payload[4096];

myip=(struct ip *)payload;
myudp=(struct udphdr *)( (char *)payload+sizeof(struct ip))


sin.sin_family=AF_INET;
sin.sin_port = htons(22);
sin.sin_addr.s_addr = inet_addr("192.168.1.1");
memset(payload, 0, 4096);

myip->ip_hl=5;
myip->ip_v=4;
myip->ip_tos=0;
myip->ip_len=sizeof(struct ip)+sizeof(struct udphdr);

myip->ip_id=htonl(6666);
myip->ip_off=0;
myip->ip_ttl=255;
myip->ip_p=17;
myip->ip_sum=0;
myip->ip_src.s_addr=inet_addr("192.168.1.12");
myip->ip_dst.s_addr=sin.sin_addr.s_addr;;
myudp->uh_sport=htons(47777);
myudp->uh_dport=htons(22);
myudp->uh_ulen=htons(sizeof(struct udphdr)+sizeof(payload));
myudp->uh_sum=0;

myip->ip_sum = csum((unsigned short *)payload, myip->ip_len >> 1);

int s = socket (AF_INET, SOCK_RAW, IPPROTO_RAW);
int one = 1;
const int *val = &one;
setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one));

sendto(s,payload,myip->ip_len,0,(struct sockaddr *)&sin, sizeof(sin));

}