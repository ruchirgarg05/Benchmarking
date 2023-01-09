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

#define SERV_PORT 6567
#define IPV4_ADDR "127.0.0.1" // Loopback Virtual Interface
#define TCP_PAYLOAD_BYTE_LEN 8

/***************************************************
 * TCP Loopback RTT Benchmarks - CLIENT
 * 
 * Program Flow:
 * 
***************************************************/
int main() 
{
    // Check for root privileges
    if (getuid() != 0)
    {
        fprintf(stderr, "This program requires root privileges!\n");
        exit(EXIT_FAILURE);
    }

    struct Timer timer;
    float total_cycles = 0;

    int send_sock_fd;
    struct sockaddr_in addr;
    char send_buff[TCP_PAYLOAD_BYTE_LEN / sizeof(char)] = "ping";
    char recv_buff[TCP_PAYLOAD_BYTE_LEN / sizeof(char)] = "ping";

    // socket()
    send_sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(send_sock_fd == -1){
        fprintf(stderr, "Socket creation error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = SERV_PORT;
    inet_pton(AF_INET, IPV4_ADDR, &addr.sin_addr.s_addr);

    // connect()
    if (connect(send_sock_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) 
    {
       fprintf(stderr, "Connect failure(%s)\n", strerror(errno));
       exit(EXIT_FAILURE);        
    }

    for (int i=0; i<NUM_ITERATIONS; i++)
    {
        if (CVERBOSE>=FULL) 
        {
            printf("To Server: %s\n", send_buff);
        }

        tic(timer);
        // write()
        if ( write(send_sock_fd, send_buff, sizeof(send_buff)) == -1)
        {
        fprintf(stderr, "write() failure(%s)\n", strerror(errno));
        exit(EXIT_FAILURE);        
        }
        bzero(recv_buff, sizeof(recv_buff));

        // read()
        if ( read(send_sock_fd, recv_buff, sizeof(recv_buff)) == -1)
        {
        fprintf(stderr, "read() failure(%s)\n", strerror(errno));
        exit(EXIT_FAILURE);        
        }
        toc(timer);

        if (CVERBOSE>=FULL) 
        {
            printf("From Server: %s\n", recv_buff);
        }

        total_cycles += (float)timer_diff(timer);
    }

    printf("Total Iterations: %d\n", NUM_ITERATIONS);
    printf("Average Number of Cycles: %f\n", total_cycles / NUM_ITERATIONS);
        

    close(send_sock_fd);
    return 0;
}