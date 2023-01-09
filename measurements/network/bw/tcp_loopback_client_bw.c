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
#include <math.h>
#include "../../../utils/utils.h"

#define SERV_PORT 6567
#define IPV4_ADDR "127.0.0.1" // Loopback Virtual Interface
#define TCP_PAYLOAD_BYTE_LEN 8

/***************************************************
 * TCP Loopback BW Benchmarks - CLIENT
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

    // Create Payload and Chunk based on bytes per TCP payload
    int total_bytes = (int)pow(2, 30);
    char* total_payload = (char*)malloc(total_bytes);
    for (int i = 0; i < total_bytes; i++) 
    {
        total_payload[i] = '!';
    }

    int payload_sizes[] = {512, 1024, 2048, 4096, 8192, 16384, 32768, 64000};

    for (int i=0; i<sizeof(payload_sizes)/sizeof(int); i++) 
    {
        int tcp_payload_byte_len = payload_sizes[i];

        printf("Sending payload with %d bytes per TCP Payload\n", tcp_payload_byte_len);

        char send_buff[tcp_payload_byte_len];

        tic(timer);
        for (int j=0; j<total_bytes/tcp_payload_byte_len;j++) {
            // printf("%d\n",j);
            strncpy(send_buff, total_payload + tcp_payload_byte_len*j, tcp_payload_byte_len);

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
        }
        toc(timer);

        printf("Cycles to send full payload (%d bytes/TCP payload): %ld \n", tcp_payload_byte_len, timer_diff(timer));
    }

    close(send_sock_fd);
    free(total_payload);
    return 0;
}