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

enum State1 {
    join, wait, create, get, st, sr, sc, leave
};

enum State2 {

    error, busy, idle

};

int net_control(struct net_info *netInfo) {

    enum State1 state = join;

    while (state != leave) {
        switch (state) {
            case join:
                TCP_server(netInfo->ext_IP, netInfo->ext_TCP);
                break;
            case wait:
                fgets(buffer, BUFFERSIZE, stdin);
                state = state_choice(buffer);
                break;
            case create:
                break;
            case get:
                break;
            case st:
                break;
            case sr:
                break;
            case sc:
                break;
            default:
                break;


        }
    }
}

int TCP_server(char *IP, char *port) {


    struct addrinfo hints, *res;
    int listen_fd, errcode, maxfd, counter;
    struct node *list_fd, *head_fd;
    fd_set rfds;
    ssize_t n_read, n_write;
    struct sockaddr addr;
    socklen_t addrlen;
    char *ptr, *buffer;
    enum State2 state;

    head_fd = create_list(0, list_fd);
    list_fd = head_fd;

    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) state = error;
    if ((buffer = (char *) malloc(BUFFERSIZE)) == NULL) state = error;

    memset(hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((errcode = getaddrinfo(NULL, port, &hints, &res)) == -1) state = error;

    if (bind(listen_fd, res->ai_addr, res->ai_addrlen) == -1) state = error;
    if (listen(listen_fd, QUEUELENGHT) == -1) state = error;
    state = idle;
    while (1/*TODO*/) {
        switch (state) {
            case idle:
                FD_SET(//TODO)
            case busy://TODO
        }
        counter = select(max_fd_list(head_fd), &rfds, (fd_set *) NULL, (fd_set *) NULL, (struct timeval *) NULL);
        if (counter =<0) state = error;

        for (; counter; --counter) {
            switch (state) {
                case idle:
                    break;
                case busy:
                    break;


            }

        }
    }
}