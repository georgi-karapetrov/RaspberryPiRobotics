#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

#define FAILURE 0
#define SUCCESS 1

int establishConnection(char * serverIP)
{
    printf("Connecting to IP: %s\n", serverIP);
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 


    memset(recvBuff, '0', sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return FAILURE;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080); 

    if(inet_pton(AF_INET, serverIP, &serv_addr.sin_addr) <= 0)
    {
        printf("\n inet_pton error occured\n");
        return FAILURE;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return FAILURE;
    } 

    while ((n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    } 

    if (n < 0)
    {
        printf("\n Read error \n");
        return FAILURE;
    } 

    return SUCCESS;
}

#define BUFFER_SIZE 64

void getIPAddressForHost(char* host, char* serverIP)
{
    char buffer[BUFFER_SIZE];

    char pingCommand[128];
    sprintf(pingCommand, "ping -c 1 %s | grep \"64 bytes from \" | awk '{print $4}' | cut -d\":\" -f1", host);
    FILE *fd = popen(pingCommand, "r");
    fgets(buffer, BUFFER_SIZE, fd);

    pclose(fd);
    printf("The IP address is %s\n", buffer);
    
    strcpy(serverIP, buffer);
}

int main(int argc, char* argv[])
{
    char hostName[64] = "raspberrypi.local";
    if (argc < 2)
    {
        printf("\n Usage: %s <Host name> \nUsing default parameter ('raspberrypi.local').\n", argv[0]);
    }
    else
    {
        hostName[0] = '\0';
        strcpy(hostName, argv[1]);
    }

    char serverIP[32];
    getIPAddressForHost(hostName, serverIP);

    int didConnect = establishConnection(serverIP);
    if (didConnect)
    {
        printf("Connection established.\n");
    }
    else
    {
        printf("Could not establish connection to %s\n", serverIP);
    }

    return 0;
}
