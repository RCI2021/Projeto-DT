//
// Created by anton on 23/03/2021.
//
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include "client.h"
#include "UDP.h"

int join(struct my_info *args, struct net_info *info) {

    char *message, *buffer;

    if ((message = (char *) malloc(BUFFERSIZE)) == NULL) return -1;
    if ((buffer = (char *) malloc(BUFFERSIZE)) == NULL) return -1;

    if ((info->ext_IP == NULL) && (info->ext_TCP == NULL)) {
        sprintf(message, "NODES %d", info->net);
        if ((UDP_exch(message, buffer, args)) != 0) {
            free(message);
            free(buffer);
            return -1;
        }
        sscanf(buffer, "%*s %*d %s %s", info->ext_IP, info->ext_TCP);
    }

    sprintf(message, "REG %d %s %s", info->net, args->IP, args->TCP);
    if ((UDP_exch(message, buffer, args)) != 0) {
        perror("UDP Error");
        free(message);
        free(buffer);
        return -1;
    }
    if (!strcmp(buffer, OKREG)) {
        perror("timeout");
        free(message);
        free(buffer);
        return -1;
    }

    free(message);
    free(buffer);
    return 0;
}

int unreg(struct my_info *args, struct net_info *info) {

    char *message, *buffer;

    if ((message = (char *) malloc(BUFFERSIZE)) == NULL) return -1;
    if ((buffer = (char *) malloc(BUFFERSIZE)) == NULL) return -1;

    sprintf(message, "UNREG %d %s %s", info->net, args->IP, args->TCP);
    UDP_exch(message, buffer, args);
    if (!strcmp(buffer, OKUNREG)) {
        perror("OKUNREG not received");
        free(message);
        free(buffer);
        return -1;
    }

    free(message);
    free(buffer);
    return 0;

}

int UDP_exch(char *message, char *buffer, struct my_info *args) {

    struct addrinfo hints, *res;
    int serverfd, errcode;
    struct sockaddr addr;
    socklen_t addrlen;
    ssize_t n;
    fd_set rfds;
    int retval;

    if ((serverfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {   //Create Socket
        perror("Error creating socket");
        return -1;
    }

    memset(&hints, 0, sizeof hints);  //Set hints
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if ((errcode = getaddrinfo(args->regIP, args->regUDP, &hints, &res)) != 0) {
        perror("Error addrinfo UDP");
        close(serverfd);
        freeaddrinfo(res);
        return -1;
    }

    if ((n = sendto(serverfd, message, sizeof message, 0, res->ai_addr, res->ai_addrlen)) == -1) {
        perror("Error sending message to UDP server");
        close(serverfd);
        freeaddrinfo(res);
        return -1;
    }

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    FD_SET(serverfd, &rfds);

    retval = select(serverfd + 1, &rfds, (fd_set *) NULL, (fd_set *) NULL, (struct timeval *) NULL);

    if (retval < 1) {
        perror("Select Error");
        return -1;
    } else if (FD_ISSET(serverfd, &rfds)) {

        addrlen = sizeof addr;
        if ((n = recvfrom(serverfd, buffer, BUFFERSIZE, 0, &addr, &addrlen)) == -1) {
            perror("Error Retriving message from UDP server");
            close(serverfd);
            freeaddrinfo(res);
            return -1;
        } else buffer[n] = '\0';

    } else if (FD_ISSET(0, &rfds)) {

        fgets(buffer, BUFFERSIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    }

    close(serverfd);
    freeaddrinfo(res);
    return 0;

}