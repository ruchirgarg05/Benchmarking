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
// #define IPV4_ADDR "45.77.195.209" // Loopback Virtual Interface
#define TCP_PAYLOAD_BYTE_LEN 8

/***************************************************
 * TCP Remote Connection Overhead Benchmarks - CLIENT
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
    addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, IPV4_ADDR, &addr.sin_addr.s_addr);

    for (int i=0; i<NUM_ITERATIONS; i++)
    {
        if (CVERBOSE>=FULL) 
        {
            printf("Opening connection\n");
        }

        send_sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        tic(timer);
        // connect()
        if (connect(send_sock_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) 
        {
            fprintf(stderr, "Connect failure(%s)\n", strerror(errno));
            exit(EXIT_FAILURE);        
        }

        if (CVERBOSE>=FULL) 
        {
            printf("Closing connection\n");
        }

        close(send_sock_fd);

        toc(timer);
        total_cycles += (float)timer_diff(timer);
    }

    printf("Total Iterations: %d\n", NUM_ITERATIONS);
    printf("Average Number of Cycles: %f\n", total_cycles / NUM_ITERATIONS);
        

    close(send_sock_fd);
    return 0;
}