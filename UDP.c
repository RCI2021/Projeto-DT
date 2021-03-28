//
// Created by anton on 23/03/2021.
//

#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include "msg.h"
#include "UDP.h"

int join(char *net, int id) {

    char *buffer;

    if ((buffer = (char *) malloc(BUFFERSIZE + 1)) == NULL) return -1;

    if ((UDPget(buffer)) != 0) {

        printf("Error UDPget");//LAMP
        return -1;
    }
    UDPreg(net, id);

}

int joinDS(char *net, int id, char *bootIP, char *bootTCP) {

    //TODO
    UDPreg(net, id);

}

int UDPget(char *buffer) {

    struct addrinfo hints, *res;
    ind serverfd, errcode;
    socklen_t addrlen;
    ssize_t n;
    char *message;

    if ((serverfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) perror("Error creating socket for UDP comms");
    return -1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (errcode = (getaddrinfo(UDPSERVER, UDPPORT, &hints, &res)) != 0) perror("Error addrinfo UDP");
    return -1;

    //INSERT TO SEND INTO MESSAGE

    if ((n = sendto(serverfd, message, sizeof message, 0, res->ai_addr, res->ai_addrlen)) == -1) {

        perror("Error sending message to UDP server");
        close(serverfd);
        freeaddrinfo(res);
        return -1;

    }

    addrlen = sizeof addr;
    if ((n = recvfrom(serverfd, buffer, BUFFERSIZE, 0, &addr, &addrlen)) == -1) perror("Error retriving msg from UDP");
    return -1;

    buffer[n] = '\0';

    //Save 1st knot
    free(message);
    close(fd);
    freeaddrinfo(res);
    return 0;
}