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
#include <signal.h>
#include <math.h>
#include "../../../utils/utils.h"

#define RECV_PORT 6567
// #define IPV4_ADDR "149.28.102.216" 
#define TCP_PAYLOAD_BYTE_LEN 8

/***************************************************
 * TCP Remote BW Benchmarks - SERVER
***************************************************/
int main() 
{
    // Check for root privileges
    if (getuid() != 0)
    {
        fprintf(stderr, "This program requires root privileges!\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGPIPE, SIG_IGN);

    struct Timer timer;
    float total_cycles = 0;

    // RECV
    int recv_sock_fd;
    int acc_sock_fd;
    struct sockaddr_in addr;
    struct sockaddr_in client_addr;
    int client_addr_len;
    char recv_buff[65536];
    char send_buff[TCP_PAYLOAD_BYTE_LEN / sizeof(char)] = "pong";

    // socket()
    recv_sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);    
    if(recv_sock_fd == -1){
        fprintf(stderr, "Socket creation error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons( RECV_PORT );
    addr.sin_addr.s_addr = INADDR_ANY;

    // bind()
    if(bind(recv_sock_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
       fprintf(stderr, "Address binding error\n");
       exit(EXIT_FAILURE);
    }

    // listen()
    if (listen(recv_sock_fd, SOMAXCONN) == -1)
    {
       fprintf(stderr, "listen() error. \n");
       exit(EXIT_FAILURE);
    }

    // Continuously Poll
    client_addr_len = sizeof(client_addr);
    for (;;) 
    {
        if (CVERBOSE>=FULL) 
        {
            printf("Server: Waiting for connections...\n");
        }

        // accept()
        acc_sock_fd = accept(recv_sock_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        printf("DEBUG: LINE201, acc_sock_ds = %d\n", acc_sock_fd);

        for (;;) 
        {
            // read()
            if ( read(acc_sock_fd, recv_buff, sizeof(recv_buff)) < 0)  // Should be a blocking call
            {
                fprintf(stderr, "read() failure(%s)\n", strerror(errno));
                exit(EXIT_FAILURE);        
            }
            if (CVERBOSE>=FULL) 
            {
                printf("From client: %s\n", recv_buff);
            }

            // write()
            if ( write(acc_sock_fd, send_buff, sizeof(send_buff)) == -1)
            {
                fprintf(stderr, "write() failure(%s)\n", strerror(errno));
                break; // Return to accept() loop
            }
        } // TODO: Graceful shutdown
    }

    close(recv_sock_fd);
    return 0;
}


