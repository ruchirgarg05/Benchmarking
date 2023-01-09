#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../../../utils/utils.h"

#define IPV4_SERV_ADDR "45.77.195.209" // Loopback Virtual Interface should be set to this
#define IPV4_CLIENT_ADDR "45.77.73.51"
#define PKT_SIZE 64

unsigned short in_cksum(unsigned short *, int);

/***************************************************
 * ICMP RTT Benchmarks - Remote SERVER
***************************************************/
int main() 
{
    // Check for root privileges
    if (getuid() != 0)
    {
        fprintf(stderr, "This program requires root privileges!\n");
        exit(EXIT_FAILURE);
    }

    int sock_fd;
    struct sockaddr_in send_addr;   // Server
    struct sockaddr_in recv_addr; // Calling client
    struct iphdr* ip;
    struct iphdr* ip_reply;
    struct icmphdr* icmp;
    int total_packet_size = sizeof(struct iphdr) + sizeof(struct icmphdr);
    int addrlen;
    int siz;

    // Build Packet
    char* packet;
    char* buffer;

    packet = malloc(sizeof(struct iphdr) + sizeof(struct icmphdr));
    buffer = malloc(sizeof(struct iphdr) + sizeof(struct icmphdr));

    ip = (struct iphdr*) packet;
    icmp = (struct icmphdr*) (packet + sizeof(struct iphdr));

    // IP Packet
    ip->ihl = 5;
    ip->version = 4;
    ip->tos = 0;
    ip->tot_len = sizeof(struct iphdr) + sizeof(struct icmphdr);
    ip->id = htons(0);
    ip->frag_off = 0;
    ip->ttl = 64;
    ip->protocol = IPPROTO_ICMP;
    inet_pton(AF_INET, IPV4_SERV_ADDR, &ip->saddr); // Spoof?
    inet_pton(AF_INET, IPV4_CLIENT_ADDR, &ip->daddr);
    ip->check = in_cksum((unsigned short *)ip, sizeof(struct iphdr));

    //ICMP Packet
    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->un.echo.id = random();
    icmp->un.echo.sequence = 0;
    icmp-> checksum = in_cksum((unsigned short *)icmp, sizeof(struct icmphdr));


    // Create Socket
    sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock_fd < 0) 
    {
        fprintf(stderr, "Could not create socket \n");
        exit(-1);
    }

    // The server
    send_addr.sin_family = AF_INET;
    inet_pton(AF_INET, IPV4_CLIENT_ADDR, &send_addr.sin_addr.s_addr);

    // Manually set...we should be parsing this out of the packet but whatever
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = INADDR_ANY;

    // Expect that most of the packets won't be lost
    for (int i = 0; i<NUM_ITERATIONS; i++) 
    {
        printf("recvfrom()\n");
        // Receive packet
        addrlen = sizeof(recv_addr);
        if ( (siz = recvfrom(sock_fd, 
                             buffer, 
                             total_packet_size, 
                             0, 
                             (struct sockaddr *)&recv_addr, 
                             &addrlen)) == -1 ) 
        {
            fprintf(stderr, "recv \n");
            exit(EXIT_FAILURE);
        }

        // Resend packet
        sendto(sock_fd, packet, ip->tot_len, 0, (struct sockaddr *)&send_addr, sizeof(struct sockaddr));
        if (CVERBOSE>=FULL) 
        {
            printf("Sent %d byte packet to %s\n", ip->tot_len, IPV4_CLIENT_ADDR);
        }

        printf("Received %d byte request.\n", siz);
        // ip_reply = (struct iphdr*) buffer;
        // printf("ID: %d\n", ntohs(ip_reply->id));
    }
    
    free(packet);
    free(buffer);
    close(sock_fd);
    return 0;
}

/*
* in_cksum --
* Checksum routine for Internet Protocol
* family headers (C Version)
*/
unsigned short in_cksum(unsigned short *addr, int len)
{
    register int sum = 0;
    unsigned short answer = 0;
    register unsigned short *w = addr;
    register int nleft = len;
    /*
    * Our algorithm is simple, using a 32 bit accumulator (sum), we add
    * sequential 16 bit words to it, and at the end, fold back all the
    * carry bits from the top 16 bits into the lower 16 bits.
    */
    while (nleft > 1)
    {
    sum += *w++;
    nleft -= 2;
    }
    /* mop up an odd byte, if necessary */
    if (nleft == 1)
    {
    *(unsigned char *) (&answer) = *(unsigned char *) w;
    sum += answer;
    }
    /* add back carry outs from top 16 bits to low 16 bits */
    sum = (sum >> 16) + (sum & 0xffff); /* add hi 16 to low 16 */
    sum += (sum >> 16); /* add carry */
    answer = ~sum; /* truncate to 16 bits */
    return (answer);
}