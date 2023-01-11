#include<arpa/inet.h>
#include<netdb.h>
#include<netinet/in.h>
#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define START 0
#define END 65535

void DieWithError(char *errorMessage);

int main(int argc, char **argv) {
  struct sockaddr_in targetServAddr;
  char *servIP;
  int sock;

  if (argc != 2)
  {
          fprintf(stderr, "Usage: %s <Server IP>\n",argv[0]);
          exit(1);
  }

  servIP = argv[1];



  printf("Start TCP Scan... \n");
  for (int i = START; i <= END; i++) {
        if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
                DieWithError("socket() failed");

        memset(&targetServAddr, 0, sizeof(targetServAddr));
        targetServAddr.sin_family = AF_INET;
        targetServAddr.sin_addr.s_addr = inet_addr(servIP);

        bool flag = false;
        int port = i;
        targetServAddr.sin_port = htons(port);        
        if (connect(sock, (struct sockaddr *) &targetServAddr, sizeof(targetServAddr)) != -1) {
                close(sock);
                flag = true;
        }

        close(sock);
        if (flag)
                printf("port %d is available\n", port);
  }
  printf("End TCP Scan.\n");
  return 0;
}