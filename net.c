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

int TCP_client(struct net_info *info, struct socket_list *list) { //RETURN FD

    fd_set rfds;
    struct addrinfo hints, *res;
    int fd, errcode;
    ssize_t nbytes, nleft, nwritten, nread;
    char *ptr, buffer[BUFFERSIZE]; //Question ptr não devia ser inteiro

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
    ptr = &buffer[0];  //question este ptr é para q?
    nbytes = (4 + sizeof(info->id));

    nleft = nbytes;
    while (nleft > 0) {
        nwritten = write(fd, ptr, nleft);
        if (nwritten <= 0) return -1;
        nleft -= nwritten;
        ptr += nwritten;
    }
    nbytes = 30; //question EXT +3?
    nleft = nbytes;
    ptr = buffer;

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    FD_SET(fd, &rfds);

    errcode = select(fd + 1, &rfds, (fd_set *) NULL, (fd_set *) NULL, (struct timeval *) NULL); ///ver situação em q n há mais nós

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
    /*if (!tree_Isempty(exp)) {

        tree_adv(exp);

    }*/

    list = insertList(list, fd);


    return fd;
}

int send_adv(int id, int fd) {

    char *ptr, buffer[13];
    int nleft, nwritten;

    sprintf(buffer, "ADVERTISE %d\n", id);

    nleft = strlen(buffer);
    ptr = buffer;

    while (nleft > 0) {
        nwritten = write(fd, ptr, nleft);
        if (nwritten <= 0) return -1;
        nleft -= nwritten;
        ptr += nwritten;
    }

    return 0;
}

int TCP_server(struct my_info *args, struct net_info *info) {

    struct addrinfo hints, *res;
    int listen_fd, new_fd, connect_fd, errcode, max_fd, count, buffer_id;
    fd_set rfds_current, rfds;
    ssize_t n_read, n_write;
    struct sockaddr addr;
    socklen_t addrlen;
    char *ptr, *buffer, *buffer_name;
    struct socket_list *head_fd = NULL, *list_fd = NULL;   //TODO inicializar logo aqui?

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

    while (/*TODO keyboard=leave*/1) {

        addrlen = sizeof addr;
        rfds_current = rfds;
        count = select(max_fd + 1, &rfds_current, (fd_set *) NULL, (fd_set *) NULL, (struct timeval *) NULL);

        if (count < 1) return -1;//TODO ERROR

        for (; count; --count) {

            if (FD_ISSET(listen_fd, &rfds_current)) {

                FD_CLR(listen_fd, &rfds);
                addrlen = sizeof addr;
                if ((new_fd = accept(listen_fd, &addr, &addrlen)) == -1) return -1;//TODO ERROR
                list_fd = insertList(head_fd, new_fd);
                FD_SET(new_fd, &rfds);

            } else if (FD_ISSET(0, &rfds_current)) {

                FD_CLR(0, &rfds);
                fgets(buffer, BUFFERSIZE, stdin);

                if (strncmp(buffer, "create", 6)) {

                    sscanf(buffer, "%*s %s", buffer); //TODO podemos usar o destino e a origem no mesmo ?
                    //add_Item(buffer, local); //Adicionar o item à base de dados local

                } else if (strncmp(buffer, "get", 3)) {

                    sscanf(buffer, "%*s %s", buffer_name);
                    sscanf(buffer_name, "%d", buffer_id);

                    if (buffer_id == info->id)
                        if (/*search_Item(buffer_name, local) < 0*/ buffer != NULL) {

                            sprintf(buffer, "NODATA %s\n", buffer_name);
                            //write(//todo, buffer, sizeof buffer);

                            /*else if(buffer != NULL){
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


                        } else if (strncmp(buffer, "show topology", sizeof "show topology") || strncmp(buffer, "st", 2)) {

                            printf("Extern: %s:%s\n Recovery: %s:%s\n", info->ext_IP, info->ext_TCP, info->rec_IP,
                                   info->rec_TCP);

                        } else if (strcmp(buffer, SR1) || strcmp(buffer, SR2)) {
                            print_Tree(TODO);
                        } else if (strcmp(buffer, SC1) || strcmp(buffer, SC2)) {

                            printf("Cache currently stored:\n1: %s \n2: %s\n", cache->name[0], cache->name[1]);

                        } else if (strcmp(buffer, LV)) {
                            //TODOlv;
                        } else //TODOkb_error;

                } else {
                    for (list_fd = head_fd; list_fd->next != NULL; list_fd = list_fd->next) {

                        if (FD_ISSET(list_fd->fd, &rfds_current)) {

                            int n; ///
                            if ((n = read(list_fd->fd, buffer, BUFFERSIZE)) != 0) {
                                if (n == -1) return -1;//TODO ERROR
                                buffer[n] = '\0';

                                if (strncmp(buffer, "EXTERN", sizeof "EXTERN")) {
                                    //TODOext;
                                } else if (strncmp(buffer, "ADVERTISE", sizeof "ADVERTISE")) {
                                    //advertise()//TODOadv;
                                } else if (strncmp(buffer, "WITHDRAW", sizeof "WITHDRAW")) {
                                    //TODOwit;
                                } else if (strncmp(buffer, "INTEREST", sizeof "INTEREST")) {
                                    //TODOinterest;
                                } else if (strncmp(buffer, "DATA", sizeof "DATA")) {
                                    //TODOd;
                                } else if (strncmp(buffer, "NODATA", sizeof "NODATA")) {
                                    //TODOnod;
                                } else //TODOerr;
                            }
                        }*/
                        }
                }
            }
        }
        return 0;
    }
}