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
#include "linked_list.h"
#include "cache.h"
#include "user_interface.h"


int TCP_client(struct net_info *info, struct socket_list *list, exp_tree **tree, struct my_info *my_info) { //RETURN FD

    fd_set rfds;
    struct addrinfo hints, *res;
    int fd, errcode, buffer_id;
    char buffer[BUFFERSIZE];

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

    sprintf(buffer, "NEW %s %s\n", my_info->IP, my_info->TCP);
    TCP_send(buffer, fd);

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
        FD_CLR(fd, &rfds);
        TCP_rcv(fd, buffer);
    }
    if (FD_ISSET(0, &rfds)) {
        FD_CLR(0, &rfds);
        fgets(buffer, BUFFERSIZE, stdin);
        if (strcmp(buffer, "cancel") != 0) {
            printf("New Operation Canceled");
            return -1;
        }
    }

    if (strncmp(buffer, "EXTERN", 6) == 0) {


        sscanf(buffer, "%*s %s %s", info->rec_IP, info->rec_TCP);
    } else printf("Expected Extern, Recieved %s", buffer);

    sprintf(buffer, "ADVERTISE %d\n", info->id);
    TCP_send(buffer, fd);
    send_tree(tree, fd);

    freeaddrinfo(res);

    list = insertList(list, fd);

    TCP_rcv(fd, buffer); //TODO error
    sscanf(buffer, "%*s %d", &buffer_id);
    tree = insert(buffer_id, fd, tree);

    return fd;
}


int TCP_server(struct my_info *args, struct net_info *info, struct socket_list *list, exp_tree **tree) {

    struct addrinfo hints, *res;
    int listen_fd, new_fd, current_fd, interest_fd = 0, errcode, max_fd, count, buffer_id, n;
    fd_set rfds_current, rfds;
    struct sockaddr addr;
    socklen_t addrlen;
    char *buffer, *buffer_name;
    struct socket_list *aux;
    struct Cache *local = NULL, *cache = NULL;


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
    if ((local = cache_init(LOCALSIZE)) == NULL) return -1;
    if ((cache = cache_init(CACHESIZE)) == NULL) return -1;

    FD_SET(0, &rfds);
    FD_SET(listen_fd, &rfds);
    max_fd = FD_setlist(list, &rfds);
    if (listen_fd > max_fd) max_fd = listen_fd;

    printf("You are now connected to net %d with id %d\n\n", info->net, info->id);

    while (strcmp(buffer, "leave\n") != 0) {

        rfds_current = rfds;

        count = select(max_fd + 1, &rfds_current, (fd_set *) NULL, (fd_set *) NULL, (struct timeval *) NULL);

        if (count < 1) return -1;//TODO ERROR

        for (; count; --count) {

            if (FD_ISSET(listen_fd, &rfds_current)) {

                addrlen = sizeof addr;
                if ((new_fd = accept(listen_fd, &addr, &addrlen)) == -1) return -1;//TODO ERROR
                if (new_fd > max_fd) max_fd = new_fd; //Atualizar o max_fd
                list = insertList(list, new_fd); //Add the new fd to the list
                FD_SET(new_fd, &rfds); //Add the new fd to the select reading array

            } else if (FD_ISSET(0, &rfds_current)) { //Does stdin have something for me to read?

                fgets(buffer, BUFFERSIZE, stdin); //Get whatever is written in stdin

                if (strncmp(buffer, "create", 6) == 0) { //Is it a create?

                    ui_create(buffer, local, info->id);

                } else if (strncmp(buffer, "get", 3) == 0) {

                    ui_get(buffer, local, cache, tree);

                } else if ((strcmp(buffer, "show topology\n") == 0) || (strcmp(buffer, "st\n") == 0)) {

                    printf("Extern: %s:%s\n Recovery: %s:%s\n", info->ext_IP,
                           info->ext_TCP, info->rec_IP, info->rec_TCP); //Print Extern and Recovery IPs & TCPs

                } else if (strcmp(buffer, "show routing\n") == 0 || strcmp(buffer, "sr\n") == 0) {

                    print_Tree(tree); //Print the tree in order, which will show as as ordered array of nodes

                } else if (strcmp(buffer, "show cache\n") == 0 || strcmp(buffer, "sc\n") == 0) {

                    if (cache->size == 0) printf("NO CACHE AVAILABLE!\n"); //Was there a Problem with cache?
                    else {
                        printf("Names stored in Cache:\n"); //Print names stored in cache
                        cache_print(cache);
                        printf("Names stored in Local:\n"); //Print names stored in local database
                        cache_print(local);
                    }
                } else if (strcmp("leave\n", buffer) != 0) {
                    printf("\nUnknown Command; Available commands are :\n\tcreate <subname>"
                           "\n\tget <name>\n\tshow topology\n\tshow routing\n\tshow cache\n");
                }
            } else if (list != NULL) { //Are there any sockets connected to me?
                for (aux = list; aux != NULL; aux = aux->next) {    //Which socket has something for me to read?

                    if (FD_ISSET(aux->fd, &rfds_current)) {

                        n = TCP_rcv(aux->fd, buffer); //Receive message
                        if (n == -1) return -1;//TODO ERROR
                        else if (n == 0) { //Read=0 means the client disconnected

                            printf("Client lost, all nodes connected to socket %d are no longer available\n", aux->fd);
                            FD_CLR(aux->fd, &rfds);
                            close(aux->fd); //Close the corresponding socket
                            tree = withdraw_tree(tree, aux->fd);
                            remove_socket(&list, aux->fd); //Remove the socket from the list
                        }
                        buffer[n] = '\0'; //Complete the message

                        if (strncmp(buffer, "NEW", 3) == 0) {

                            extern_update(info, args, buffer);
                            sprintf(buffer, "EXTERN %s %s\n", info->ext_IP, info->ext_TCP); //Create Extern message
                            TCP_send(buffer, aux->fd); //Send Extern message
                            if ((n = TCP_rcv(aux->fd, buffer)) <= 0) perror("Advertise ERROR");
                            buffer[n] = '\0';
                            TCP_send_all(buffer, list, aux->fd);
                            sscanf(buffer, "%*s %d", &buffer_id);
                            sprintf(buffer, "ADVERTISE %d\n", info->id);
                            TCP_send(buffer, aux->fd);
                            send_tree(tree, aux->fd); //Advertise tree to new node
                            tree = insert(buffer_id, aux->fd, tree);


                        } else if (strncmp(buffer, "ADVERTISE", 9) == 0) {

                            sscanf(buffer, "%*s %d", &buffer_id);
                            tree = insert(buffer_id, aux->fd, tree);
                            TCP_send_all(buffer, list, aux->fd);

                        } else if (strncmp(buffer, "WITHDRAW", 8) == 0) {

                            sscanf(buffer, "%*s %d", &buffer_id);
                            tree = del_tree(buffer_id, tree);
                            TCP_send_all(buffer, list, aux->fd);

                        } else if (strncmp(buffer, "INTEREST", 8) == 0) {

                            sscanf(buffer, "%*s %s", buffer_name);
                            if (cache_search(buffer_name, local) >= 0) {

                                sprintf(buffer, "DATA %s", buffer_name); //If name was found send back DATA message
                                TCP_send(buffer, aux->fd);

                            } else {
                                //Continues sending the INTEREST message towards the destination
                                sscanf(buffer, "%*s %d", &buffer_id);
                                current_fd = find_socket(buffer_id, tree);
                                interest_fd = current_fd; //Save the fd in which the request came for later
                                if (current_fd != -1) TCP_send(buffer, current_fd);
                                else {
                                    sprintf(buffer, "NODATA %s",
                                            buffer_name); //If it can´t find the destination, send NODATA
                                    TCP_send(buffer, aux->fd);
                                }

                            }

                        } else if (strncmp(buffer, "DATA", 4) == 0) {

                            sscanf(buffer, "%*s %s", buffer_name);

                            if (interest_fd != 0) {

                                cache_add(buffer_name, cache);
                                TCP_send(buffer, interest_fd);

                            } else printf("DATA %s\n", buffer_name);


                        } else if (strncmp(buffer, "NODATA", sizeof 6) == 0) {

                            sscanf(buffer, "%*s %s", buffer_name);

                            if (interest_fd != 0) TCP_send(buffer, interest_fd);
                            else printf("NODATA %s\n", buffer_name);

                        }
                    }
                }
            }
        }
    }

    close(listen_fd);
    close_list(list);
    return 0;
}


void TCP_send(char *buffer, int fd) {

    char *ptr = buffer;
    ssize_t nleft, nwritten;
    nleft = strlen(buffer);

    while (nleft > 0) {
        nwritten = write(fd, ptr, nleft);
        nleft -= nwritten;
        ptr += nwritten;
    }
}

void TCP_send_all(char *buffer, struct socket_list *list, int fd) {

    struct socket_list *aux = list;

    for (; aux->next != NULL; aux = aux->next) {

        if (fd != aux->fd) TCP_send(buffer, aux->fd);
    }
}

int TCP_rcv(int fd, char *buffer) {

    char *ptr = buffer;
    ssize_t nread, nleft = BUFFERSIZE;

    while (nleft > 0) {
        nread = read(fd, ptr, nleft);
        if (nread <= 0) break;
        nleft -= nread;
        ptr += nread;
        if (buffer[nread - 1] == '\n') break;
    }

    return nread;
}

int extern_update(struct net_info *info, struct my_info *args, char *buffer) {
    char aux_IP[IPSIZE], aux_TCP[TCPSIZE];

    if (sscanf(buffer, "%*s %s %s", aux_IP, aux_TCP) != 2) {
        perror("Strange NEW message received");
        return -1;
    }

    if (strcmp(args->IP, info->ext_IP) == 0 && strcmp(args->TCP, info->ext_TCP) == 0) {

        strcpy(info->ext_IP, aux_IP);
        strcpy(info->ext_TCP, aux_TCP);

    }

    return 0;
}