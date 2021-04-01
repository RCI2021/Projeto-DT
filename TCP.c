//
// Created by anton on 31/03/2021.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include "client.h"
#include "UDP.h"


int TCPinit(char *IP, char *port) {

    struct addrinfo hints, *res;
    int nodefd, n;
    bool err;

    nodefd = socket(AF_INET, SOCK_STREAM, 0);
    if (nodefd == -1) err = TRUE;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    n = getaddrinfo(IP, port, &hints, &res);
    if (n != 0) err = true;

    n = connect(nodefd, res->ai_addr, res->ai_addrlen);
    if (n == -1) err = true;

    n = bind(nodefd, res->ai_addr, res->ai - addrlen);
    if (n == -1) err = true;

    if (err == true) return -1;
    else return nodefd;
}