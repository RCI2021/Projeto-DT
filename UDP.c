//
// Created by anton on 23/03/2021.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include "msg.h"
#include "UDP.h"

int join(char *net, int id){

    char *buffer;

    if((buffer=(char *) malloc(BUFFERSIZE+1))==NULL) return -1;

    if ((UDPget(buffer))!=0) {

        printf("Error UDPget");//LAMP
        return -1;
    }
    UDPreg(net,id);

}

int joinDS(char *net, int id,char *bootIP, char *bootTCP){

    //TODO
    UDPreg(net,id);

}

int UDPget(char *buffer) {

    struct addrinfo hints, *res;
    int fd, errcode;
    socklen_t addrlen;
    ssize_t n;
    char host[NI_MAXHOST], service[NI_MAXSERV],*message;

    if (message= (char *) malloc (sizeof (NODESLIST)+sizeof (net))) return -1;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) return -1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (errcode = (getaddrinfo(UDPSERVER, *UDPPORT, &hints, &res)) != 0) {
        printf("Error getaddrinfo %s", gai_strerror(errcode));
        return -1;
    }

    message=strcat(NODESLIST,net);

    n = sendto(fd, message, sizeof(message), 0, res->ai_addr, res->ai_addrlen);
    if (n == -1) {
        printf("Error sendto");
        close(fd);
        freeaddrinfo(res);
        return -1;
    }

    addrlen = sizeof addr;
    if ((n = recvfrom(fd, buffer, BUFFERSIZE, 0, &addr, &addrlen)) == -1) return -1;
    buffer[n] = '\0';

    printf("echo: %s\n", buffer); //LAMP
    if ((errorcode = getnameinfo(&addr, addrlen, host, sizeof host, service, sizeof service, 0)) != 0) {

        printf("Error getnameinfo %s\n", gai_strerror(errorcode));
        close(fd);
        freeaddrinfo(res);
        return -1;

    } else printf("sent by: [%s:%s]\n", host, service); //LAMP

    free(message);
    close(fd);
    freeaddrinfo(res);
    return 0;
}