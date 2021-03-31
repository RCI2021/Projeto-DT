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

enum state {
    reg, unreg, list, connect
};

int join(char *command) {

    char *buffer, *message, *net, *id, *tcp, *nodeip, *nodetcp;

    state = reg;

    if ((joinAlloc(buffer, message, net, id, tcp, nodeip, nodetcp)) != 0) state = error;

    switch state {
        case reg:
            sscanf(command, "%*s %s %s %s", net, id, tcp);
            ssprintf(message, "%s %s %s %s", REG, net, id, tcp);
            if (UDPcomms(message, buffer) != 0) state = error;
            if (!strcmp(buffer, OKREG)) state = error;
            else state = list;
            break;

        case list:
            ssprintf(message, "%s %s", NODESLIST, net)
            if (UDPcomms(message, buffer) != 0) state = error;
            if (sscanf(buffer, "NODESLIST %*d\n %s %s", nodeip, nodetcp) != 2) state = error;
            else state = advertise;
            break;

        case connect:
            break;

        case error:
            perror("Error during Nodes Server Operations");
            joinFree(buffer, message, net, id, tcp, nodeip, nodetcp);
            break;
    }

    joinFree(buffer, message, net, id, tcp, nodeip, nodetcp);
    return 0;
}

int joinAlloc(char *buffer, char *message, char *net, char *ip, char *tcp, char *nodeip, char *nodetcp) {

    if ((buffer = (char *) malloc(sizeof BUFFERSIZE + 1)) == NULL) perror("MEM ALLOCATION ERROR");
    return -1;
    if ((message = (char *) malloc(sizeof NODESLIST + sizeof net)) == NULL) perror("MEM ALLOCATION ERROR");
    return -1;
    if ((net = (char *) malloc(sizeof BUFFERSIZE + 1)) == NULL) perror("MEM ALLOCATION ERROR");
    return -1;
    if ((ip = (char *) malloc(sizeof BUFFERSIZE + 1)) == NULL) perror("MEM ALLOCATION ERROR");
    return -1;
    if ((tcp = (char *) malloc(sizeof BUFFERSIZE + 1)) == NULL) perror("MEM ALLOCATION ERROR");
    return -1;
    if ((nodeip = (char *) malloc(sizeof BUFFERSIZE + 1)) == NULL) perror("MEM ALLOCATION ERROR");
    return -1;
    if ((nodetcp = (char *) malloc(sizeof BUFFERSIZE + 1)) == NULL) perror("MEM ALLOCATION ERROR");
    return -1;

    return 0;
}

void joinFree(char *buffer, char *message, char *net, char *ip, char *tcp, char *nodeip, char *nodetcp) {

    free(buffer);
    free(message);
    free(net);
    free(ip);
    free(tcp);
    free(nodeip);
    free(nodetcp);

}

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
        return -1;
        buffer[n] = '\0';
    }

    close(fd);
    freeaddrinfo(res);
    return 0;
}