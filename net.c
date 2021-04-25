//
// Created by António on 20/04/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include "net.h"
#include "registration.h"
#include "linked_list.h"
#include "cache.h"


int TCP_client(struct net_info *info, struct socket_list *list, exp_tree *tree) { //RETURN FD

    fd_set rfds;
    struct addrinfo hints, *res;
    int fd, errcode;
    ssize_t nbytes, nleft, nwritten, nread;
    char *ptr, buffer[BUFFERSIZE];

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) return -1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    errcode = getaddrinfo(info->ext_IP, info->ext_TCP, &hints, &res);
    if (errcode != 0) return -1;

    errcode = connect(fd, res->ai_addr, res->ai_addrlen);
    if (errcode == -1) {
        return -1;
    }

    sprintf(buffer, "NEW %d\n", info->id);
    ptr = &buffer[0];
    nbytes = (4 + sizeof(info->id));

    nleft = nbytes;
    while (nleft > 0) { //TODO use functions
        nwritten = write(fd, ptr, nleft);
        if (nwritten <= 0) return -1;
        nleft -= nwritten;
        ptr += nwritten;
    }
    nbytes = BUFFERSIZE;
    nleft = nbytes;
    ptr = buffer;

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    FD_SET(fd, &rfds);

    errcode = select(fd + 1, &rfds, (fd_set *) NULL, (fd_set *) NULL,
                     (struct timeval *) NULL); ///ver situação em q n há mais nós

    if (errcode <= 0) {
        perror("Select error");
        freeaddrinfo(res);
        return -1;
    }

    if (FD_ISSET(fd, &rfds)) {
        while (nleft > 0) { //TODO use functions
            nread = read(fd, ptr, nleft);
            if (nread == -1) return -1;
            else if (nread == 0) break;
            nleft -= nread;
            ptr += nread;
            if (buffer[nread - 1] == '\n') break;
        }
        nread = nbytes - nleft;
        buffer[nread] = '\0';
    }
    if (FD_ISSET(0, &rfds)) {
        fgets(buffer, BUFFERSIZE, stdin);
        if (strcmp(buffer, "cancel") != 0) {
            printf("New Operation Canceled");
            return -1;
        }
    }

    if (strncmp(buffer, "EXTERN", 6) != 0) {

        printf("Expected Extern, received %s", buffer);
        freeaddrinfo(res);
        return -1;

    } else sscanf(buffer, "%*s %s %s", info->rec_IP, info->rec_TCP);

    freeaddrinfo(res);

    send_tree(tree, fd);
    list = insertList(list, fd);

    return fd;
}


int TCP_server(struct my_info *args, struct net_info *info, struct socket_list *list, exp_tree *tree) {

    struct addrinfo hints, *res;
    int listen_fd, new_fd, current_fd, interest_fd, errcode, max_fd, count, buffer_id, n;
    fd_set rfds_current, rfds;
    struct sockaddr addr;
    socklen_t addrlen;
    char *buffer, *buffer_name;
    struct socket_list *aux;
    struct Cache local, cache;

    if (cache_init(local, LOCALSIZE) != LOCALSIZE) return -1;
    if (cache_init(cache, CACHESIZE) != CACHESIZE) return -1;

    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) return -1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((errcode = getaddrinfo(NULL, args->TCP, &hints, &res)) == -1) return -1;

    if (bind(listen_fd, res->ai_addr, res->ai_addrlen) == -1) return -1;
    if (listen(listen_fd, 5) == -1) return -1;

    if ((buffer = (char *) malloc(BUFFERSIZE)) == NULL) return -1;
    if ((buffer_name = (char *) malloc(BUFFERSIZE)) == NULL) return -1;

    FD_SET(0, &rfds);
    FD_SET(listen_fd, &rfds);
    max_fd = FD_setlist(list, &rfds);
    if (listen_fd > max_fd) max_fd = listen_fd;

    while (strcmp(buffer, "leave\n") != 0) {

        addrlen = sizeof addr;
        rfds_current = rfds;

        count = select(max_fd + 1, &rfds_current, (fd_set *) NULL, (fd_set *) NULL, (struct timeval *) NULL);

        if (count < 1) return -1;//TODO ERROR

        for (; count; --count) {

            if (FD_ISSET(listen_fd, &rfds_current)) {

                addrlen = sizeof addr;
                if ((new_fd = accept(listen_fd, &addr, &addrlen)) == -1) return -1;//TODO ERROR
                FD_SET(new_fd, &rfds);
                list = insertList(list, new_fd);

            } else if (FD_ISSET(0, &rfds_current)) {

                fgets(buffer, BUFFERSIZE, stdin);

                if (strncmp(buffer, "create", 6) == 0) {

                    sscanf(buffer, "%*s %s", &buffer_name);
                    cache_add(buffer_name, local);

                } else if (strncmp(buffer, "get", 3) == 0) {

                    sscanf(buffer, "%*s %s", buffer_name);
                    sscanf(buffer_name, "%d", &buffer_id);

                    if (buffer_id == info->id) {
                        if (cache_search(buffer_name, local) > 0) printf("DATA %s", buffer_name);
                        else printf("NODATA %s", buffer_name);

                    } else {
                        if (cache_search(buffer_name, cache) > 0) printf("DATA %s", buffer_name);
                        else {

                            sprintf(buffer, "INTEREST %s");
                            TCP_send(buffer, find_socket(buffer_id, tree));
                            interest_fd = 0;

                        }

                    }

                } else if ((strcmp(buffer, "show topology\n") == 0) || (strcmp(buffer, "st\n") == 0)) {

                    printf("Extern: %s:%s\n Recovery: %s:%s\n", info->ext_IP, info->ext_TCP, info->rec_IP,
                           info->rec_TCP);

                } else if (strcmp(buffer, "show routing\n") == 0 || strcmp(buffer, "sr\n") == 0) {
                    print_Tree(tree);
                } else if (strcmp(buffer, "show cache\n") == 0 || strcmp(buffer, "sc\n") == 0) {

                    printf("Names stored in Local:\n");
                    cache_print(local);
                    printf("Names stored in Cache:\n")
                    cache_print(cache);

                }
            } else {
                for (aux = list; aux->next != NULL; aux = aux->next) {

                    if (FD_ISSET(aux->fd, &rfds_current)) {

                        if ((n = TCP_rcv(aux->fd, buffer)) != 0) { //Recieve message
                            if (n == -1) return -1;//TODO ERROR
                            buffer[n] = '\0';

                        } else if (strncmp(buffer, "ADVERTISE", sizeof "ADVERTISE")) {

                            sscanf(buffer, "%*s %d", &buffer_id);
                            insert(buffer_id, aux->fd, tree);
                            TCP_send_all(buffer, list, aux->fd);

                        } else if (strncmp(buffer, "WITHDRAW", sizeof "WITHDRAW")) {

                            sscanf(buffer, "%*s %d", &buffer_id);
                            delete(buffer_id, tree);
                            TCP_send_all(buffer, list, aux->fd);

                        } else if (strncmp(buffer, "INTEREST", sizeof "INTEREST")) {
                            if (cache_search(buffer_name, local) > 0) {
                                sprintf(buffer, "DATA %s", buffer_name);
                                TCP_send(buffer, interest_fd);
                            } else {
                                sscanf(buffer, "%*s %d", &buffer_id);
                                if (current_fd = find_socket(buffer_id, tree) != -1) TCP_send(buffer, current_fd);

                            }

                        } else if (strncmp(buffer, "DATA", sizeof "DATA")) {
                            sscanf("%*s %s", buffer_name);
                            cache_add(buffer_name, cache);
                            TCP_send(buffer, interest_fd);
                        } else if (strncmp(buffer, "NODATA", sizeof "NODATA")) {
                            //TODOnod;
                        }
                    }
                }

            }
        }
    }

    return 0;
}


void TCP_send(char *buffer, int fd) {

    char *ptr = buffer;
    int nleft, nwritten;
    nleft = strlen(buffer);

    while (nleft > 0) {
        nwritten = write(fd, ptr, nleft);
        nleft -= nwritten;
        ptr += nwritten;
    }

    return;
}

void TCP_send_all(char *buffer, struct socket_list *list, int fd) {

    struct socket_list *aux = list;

    for (; aux->next != NULL; aux = aux->next) {

        if (fd != aux->fd) TCP_send(buffer, aux->fd);

    }
    return;
}

int TCP_rcv(int fd, char *buffer) {

    char *ptr = buffer;
    int nread, nleft;

    while (nleft > 0) {
        nread = read(fd, ptr, nleft);
        if (nread <= 0) break;
        nleft -= nread;
        ptr += nread;
        if (buffer[nread - 1] == '\n') break;
    }

    return nread;
}