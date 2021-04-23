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
    if (errcode == -1) return -1;

    sprintf(buffer, "NEW %d\n", info->id);
    ptr = &buffer[0];
    nbytes = (4 + sizeof(info->id));

    nleft = nbytes;
    while (nleft > 0) {
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
        while (nleft > 0) {
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
    int listen_fd, new_fd, current_fd, errcode, max_fd, count, buffer_id;
    fd_set rfds_current, rfds;
    struct sockaddr addr;
    socklen_t addrlen;
    char *buffer, *buffer_name;
    struct socket_list *aux;

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
    max_fd = FD_setlist(list, &rfds);

    while (strcmp(buffer, "leave\n") != 0) {

        addrlen = sizeof addr;
        rfds_current = rfds;

        count = select(max_fd + 1, &rfds_current, (fd_set *) NULL, (fd_set *) NULL, (struct timeval *) NULL); //TODO fd stdin está a ser apagado?

        if (count < 1) return -1;//TODO ERROR

        for (; count; --count) {

            if (FD_ISSET(listen_fd, &rfds_current)) {

                FD_CLR(listen_fd, &rfds);
                addrlen = sizeof addr;
                if ((new_fd = accept(listen_fd, &addr, &addrlen)) == -1) return -1;//TODO ERROR
                FD_SET(new_fd, &rfds);

            } else if (FD_ISSET(0, &rfds_current)) {

                FD_CLR(0, &rfds);
                fgets(buffer, BUFFERSIZE, stdin);

                if (strncmp(buffer, "create", 6) == 0) {

                    sscanf(buffer, "%*s %s", buffer); //TODO podemos usar o destino e a origem no mesmo ?
                    //add_Item(buffer, local); //Adicionar o item à base de dados local

                } else if (strncmp(buffer, "get", 3) == 0) {

                    sscanf(buffer, "%*s %s", buffer_name);
                    sscanf(buffer_name, "%d", &buffer_id);

                    if (buffer_id == info->id) {
                        /*if (search_Item(buffer_name, local) < 0 buffer != NULL) {

                            sprintf(buffer, "NODATA %s\n", buffer_name);
                            write(//todo, buffer, sizeof buffer);

                            else if(buffer != NULL){
                                sprintf(buffer, "DATA %s\n", buffer_name);
                                write(todo, buffer, sizeof buffer);

                            }else if (search_Item(buffer_name, cache) < 0) {

                                //TODO get fd of next knot to send
                                sprintf(buffer, "%s %s\n", INTEREST, buffer_name);
                                write(todo, buffer, sizeof buffer);

                            } else {

                                sprintf(buffer, "%s %s\n", D, buffer_name);
                                write(/TODO, buffer, sizeof buffer);
                            }

                        }*/} else {

                        if ((current_fd = find_socket(buffer_id, tree)) < 1) return -1;//TODO error
                        TCP_send(buffer, current_fd);

                    }

                } else if ((strcmp(buffer, "show topology\n") == 0) || (strcmp(buffer, "st") == 0)) {

                    printf("Extern: %s:%s\n Recovery: %s:%s\n", info->ext_IP, info->ext_TCP, info->rec_IP,
                           info->rec_TCP);

                } else if (strcmp(buffer, "show routing\n") == 0 || strcmp(buffer, "sr\n") == 0) {
                    print_Tree(tree);
                } else if (strcmp(buffer, "show cache\n") == 0 || strcmp(buffer, "sc\n") == 0) {
                    //TODO
                }
            } else {
                for (aux = list; aux->next != NULL; aux = aux->next) {

                    if (FD_ISSET(aux->fd, &rfds_current)) {

                        int n; ///
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
                            //TODOinterest;
                        } else if (strncmp(buffer, "DATA", sizeof "DATA")) {
                            //TODOd;
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