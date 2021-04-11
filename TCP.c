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
#include "TCP.h"
#include "list.h"


enum Net_action {

    ext, adv, wit, interest, d, nod, err

};

enum Keyboard_state {

    crt, get, st, sr, sc, lv, kb_error

};

int TCP_client(struct net_info *info) {

    int fd;

}

int TCP_server(struct net_info *info) {

    struct addrinfo hints, *res;
    int listen_fd, new_fd, connect_fd, errcode, max_fd, count;
    fd_set rfds_current, rfds;
    ssize_t n_read, n_write;
    struct sockaddr addr;
    socklen_t addrlen;
    char *ptr, *buffer;
    struct node *head_fd, *list_fd;
    enum Keyboard_state keyboard_state;
    enum Net_action net_state;

    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) return -1;
    if ((connect_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) return -1;

    memset(hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((errcode = getaddrinfo(NULL, port, &hints, &res)) == -1) return -1;

    if (bind(listen_fd, res->ai_addr, res->ai_addrlen) == -1) return -1;
    if (listen(listen_fd, QUEUELENGHT) == -1) return -1;

    head_fd = create_list();
    list_fd = head_fd;
    if ((buffer = (char *) malloc(BUFFERSIZE)) == NULL) return -1;

    max_fd = FD_setlist(head_fd, &rfds);
    FD_SET(0, &rfds);

    while (/*TODO keyboard=leave*/) {
        addrlen = sizeof addr;
        rfds_current = rfds;
        count = select(max_fd + 1, &rfds_current, (fd_set *) NULL, (fd_set *) NULL, (struct timeval *) NULL);

        if (count < 1) return -1;//TODO ERROR

        for (; count; --count) {

            if (FD_ISSET(listen_fd, &rfds_current)) {

                FD_CLR(fd, &rfds);
                addrlen = sizeof addr;
                if ((new_fd = accept(listen_fd, &addr, &addrlen)) == -1) return -1;//TODO ERROR
                add_item_list(new_fd, head_fd);
                FD_SET(new_fd, &rfds);

            } else if (FD_ISSET(0, &rfds_current)) {

                FD_CLR(0, &rfds);
                fgets(buffer, BUFFERSIZE, stdin);
                keyboard_state = net_keyboard_handle(buffer);

            } else {
                for (list_fd = head_fd; list_fd->next != NULL; list_fd = list_fd->next) {

                    if (FD_ISSET(list_fd->fd, &rfds_current)) {

                        if ((n = read(list_fd->fd, buffer, BUFFERSIZE)) != 0) {
                            if (n == -1) return -1;//TODO ERROR
                            net_action = net_buffer_handle(buffer);
                            switch (net_action) {
                                case ext:
                                    send_ext(); //TODO trow enum out
                                    break;
                                case adv:
                                    advert();
                                    break;
                                case wit:
                                    withdraw();
                                    break;
                                case interest:

                            }

                        }
                    }
                }
            }
        }
    }
}

enum Net_action net_buffer_handle(char *buffer) {

    if (strcmp(buffer, EXT)) {
        return ext;
    } else if (strcmp(buffer, ADV)) {
        return adv;
    } else if (strcmp(buffer, WIT)) {
        return wit;
    } else if (strcmp(buffer, INTEREST)) {
        return interest;
    } else if (strcmp(buffer, D)) {
        return d;
    } else if (strcmp(buffer, NOD)) {
        return nod;
    } else return err;
}

enum Keyboard_state net_keyboard_handle(char *buffer) {

    if (strcmp(buffer, CREATE)) {
        return crt;
    } else if (strcmp(buffer, GET)) {
        return get;
    } else if (strcmp(buffer, ST1) || strcmp(buffer, ST2)) {
        return st;
    } else if (strcmp(buffer, SR1) || strcmp(buffer, SR2)) {
        return sr;
    } else if (strcmp(buffer, SC1) || strcmp(buffer, SC2)) {
        return sc;
    } else if (strcmp(buffer, LV)) {
        return lv;
    } else return kb_error;


};