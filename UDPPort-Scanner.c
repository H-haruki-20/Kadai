#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#include<stdbool.h>
#include "errqueue.h"
#define MAXSIZE 1024     /* Longest string to echo */



void DieWithError(char *errorMessage);  /* External error handling function */

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short targetServPort;     /* Echo server port */
    char *servIP;                    /* IP address of server */
    int status;

    if (argc != 3)    /* Test for correct number of arguments */
    {
        fprintf(stderr,"Usage: %s <Server IP> <Target PORT>\n", argv[0]);
        exit(1);
    }

    servIP = argv[1];           /* First arg: server IP address (dotted quad) */
    targetServPort = atoi(argv[2]);

    bool flag = true;
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    int out = 1;
    setsockopt(sock,IPPROTO_IP,IP_RECVERR,(char*)&out,sizeof(out));

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));    /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                 /* Internet addr family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);  /* Server IP address */
    echoServAddr.sin_port   = htons(targetServPort);     /* Server port */

    /*Send the string to the server */
    if (sendto(sock, "hi", 2, 0, (struct sockaddr *)
               &echoServAddr, sizeof(echoServAddr)) != 2)
        DieWithError("sendto() sent a different number of bytes than expected");

    char buffer[MAXSIZE];
    struct iovec iov;                       /* Data array */
    struct msghdr msg;                      /* Message header */
    struct cmsghdr *cmsg;                   /* Control related data */
    struct sock_extended_err *sock_err;     /* Struct describing the error */
    struct icmphdr icmph;                   /* ICMP header */

    iov.iov_base = &icmph;
    iov.iov_len = sizeof(icmph);
    msg.msg_name = (void*)&echoServAddr;
    msg.msg_namelen = sizeof(echoServAddr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_flags = 0;
    msg.msg_control = buffer;
    msg.msg_controllen = sizeof(buffer);
    status = recvmsg(sock, &msg, MSG_ERRQUEUE);
    int err = 0;

    for(int count=0;count<10;count++)
        for (cmsg = CMSG_FIRSTHDR(&msg);cmsg; cmsg = CMSG_NXTHDR(&msg, cmsg))
        {
            if (cmsg->cmsg_level == IPPROTO_IP)
            {
                if (cmsg->cmsg_type == IP_RECVERR)
                {
                    //fprintf(stderr, "We got IP_RECVERR message\n");
                    sock_err = (struct sock_extended_err*)CMSG_DATA(cmsg);
                    if (sock_err)
                    {
                        if (sock_err->ee_origin == SO_EE_ORIGIN_ICMP && sock_err->ee_type == ICMP_DEST_UNREACH)
                        {
                            switch (sock_err->ee_code)
                            {
                                case ICMP_PORT_UNREACH:
                                    //fprintf(stderr, "Port Unreachable Error!\n");
                                    err += 1;
                                    break;
                            }
                        }
                    }
                }
            }
        }

    if(err<3)
        printf("port %d is available: %d err\n",targetServPort,err);

    memset(buffer,0,sizeof(buffer));
    close(sock);

    return 0;
}