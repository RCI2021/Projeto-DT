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


int join_net(char *command, struct S_args *args, struct net_info *netinfo) {

    char *buffer, *message, *id, *tcp, *nodeip, *nodetcp;
    int net_id;
    enum State state;
    state = reg;

    if ((joinAlloc(buffer, message, id, tcp, nodeip, nodetcp)) != 0) state = error;

    while (state != done) {
        switch (state) {
            case reg:
                sscanf(command, "%*s %d %d"
                net_id, id);
                sprintf(message, "%s %d %s %s", REG, net_id, args->IP, args->TCP);
                if (UDPcomms(message, buffer, args) != 0) state = error;
                if (!strcmp(buffer, OKREG)) state = error;
                else state = list;
                break;

            case list:
                sprintf(message, "%s %d", NODESLIST, net_id);
                if (UDPcomms(message, buffer) != 0) state = error;
                else state = get_peer;
                break;

            case error:
                perror("Error during Nodes Server Operations");
                joinFree(buffer, message, net_id, id, tcp, nodeip, nodetcp);
                net_id = -1;
                state = done;
                break;

            case get_peer:
                sscanf(buffer, "%*s %*s\n %s %s", netinfo->ext_IP, netinfo->ext_TCP);
                state = done;
                break;
            default:
                break;
        }
    }

    joinFree(buffer, message, id, tcp, nodeip, nodetcp);
    return net_id;
}

int joinAlloc(char *buffer, char *message, char *ip, char *tcp, char *nodeip, char *nodetcp) {

    if ((buffer = (char *) malloc(sizeof BUFFERSIZE + 1)) == NULL) perror("MEM ALLOCATION ERROR");
    return -1;
    if ((message = (char *) malloc(sizeof NODESLIST + sizeof net_id)) == NULL) perror("MEM ALLOCATION ERROR");
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

void joinFree(char *buffer, char *message, char *ip, char *tcp, char *nodeip, char *nodetcp) {

    free(buffer);
    free(message);
    free(ip);
    free(tcp);
    free(nodeip);
    free(nodetcp);

}

int UDPcomms(char *message, char *buffer, struct S_args *args) {

    struct addrinfo hints, *res;
    int serverfd, errcode;
    struct sockaddr addr;
    socklen_t addrlen;
    ssize_t n;
    fd_set r_fd;
    struct timeval tv;
    int retval;

    if ((serverfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Error creating socket for UDP comms");
        return -1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;


    errcode = getaddrinfo(UDPSERVER, UDPPORT, &hints, &res); //TODO
    if (errcode != 0) {
        perror("Error addrinfo UDP");
        return -1;
    }

    if ((n = sendto(serverfd, message, sizeof message, 0, res->ai_addr, res->ai_addrlen)) == -1) {

        perror("Error sending message to UDP server");
        close(serverfd);
        freeaddrinfo(res);
        return -1;
    }

    FD_ZERO(&r_fd);
    FD_SET(0, &r_fd);
    FD_SET(serverfd, &r_fd);

    tv.tv_sec = TIMEOUT;

    retval = select(serverfd + 1, &r_fd, (fd_set *) NULL, (fd_set *) NULL, &tv);

    if (retval < 1) {
        perror("Select Error");
        return -1;
    } else if (FD_ISSET(0, &r_fd)) {

        fgets(buffer, BUFFERSIZE, stdin);
        buffer[n] = '\0';

    } else if (FD_ISSET(serverfd, &r_fd)) {
        addrlen = sizeof addr;
        if ((n = recvfrom(serverfd, buffer, BUFFERSIZE, 0, &addr, &addrlen)) == -1)
            perror("Error retriving msg from UDP");
        return -1;
        buffer[n] = '\0';
    }

    close(serverfd);
    freeaddrinfo(res);
    return 0;
}