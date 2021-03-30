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
#include "client.h"
#include "UDP.h"

enum state {
    reg, unreg, list, connect
};


int UDPcomms(char *message, char *buffer) {

    struct addrinfo hints, *res;
    ind serverfd, errcode;
    socklen_t addrlen;
    ssize_t n;
    fd_set r_fd;
    struct timeval tv;
    int retval;

    if ((serverfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) perror("Error creating socket for UDP comms");
    return -1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (errcode = (getaddrinfo(UDPSERVER, UDPPORT, &hints, &res)) != 0) perror("Error addrinfo UDP");
    return -1;

    if ((n = sendto(serverfd, message, sizeof message, 0, res->ai_addr, res->ai_addrlen)) == -1) {

        perror("Error sending message to UDP server");
        close(serverfd);
        freeaddrinfo(res);
        return -1;
    }

    FD_ZERO(&r_fd);
    FD_SET(0, &r_fd);
    FD_SET(serverfd, &r_fd);

    tv.time_t = TIMEOUT;

    retval = select(serverfd + 1, r_fd, NULL, NULL, &tv);

    if (retval == -1) perror("Select Error");
    return -1;

    else if (FD_ISSET(0, &r_fd)) {

        n = fgets(buffer, BUFFERSIZE, stdin);
        buffer[n] = '\0';
        if (!strcmp(buffer, "cancel")) {
            //CANCEL WAIT
        }

    } else if (FD_ISSET(serverfd, &r_fd)) {
        addrlen = sizeof addr;
        if ((n = recvfrom(serverfd, buffer, BUFFERSIZE, 0, &addr, &addrlen)) == -1)
            perror("Error retriving msg from UDP");
        return -1; //How to end select? TODO
        buffer[n] = '\0';
    }

    close(fd);
    freeaddrinfo(res);
    return 0;
}