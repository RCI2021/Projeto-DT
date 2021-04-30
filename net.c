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
    char buffer[BUFFERSIZE], delim[2] = "\n";

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
        printf("<received -> %s>\n", buffer);  //TODO REMOVE
    }
    if (FD_ISSET(0, &rfds)) {
        FD_CLR(0, &rfds);
        fgets(buffer, BUFFERSIZE, stdin);
        if (strcmp(buffer, "cancel") != 0) {
            printf("New Operation Canceled");
            return -1;
        }
    }
    if (strtok(buffer, delim) != NULL) {
        do {
            if (strncmp(buffer, "EXTERN", 6) == 0) {
                sscanf(buffer, "%*s %s %s", info->rec_IP, info->rec_TCP);

                sprintf(buffer, "ADVERTISE %d\n", info->id);
                TCP_send(buffer, fd);
                send_tree(*tree, fd, info->id);
            } else if (strncmp(buffer,"ADVERTISE",9)==0){

                sscanf(buffer,"%*s %d",&buffer_id);
                *tree= insert(buffer_id,fd,*tree);

            }else printf("Expected Extern or Advertise, Recieved %s",buffer);
        } while (strtok(NULL, delim) != NULL);
    } else printf("Expected Extern or Advertise, Recieved %s",buffer);

    freeaddrinfo(res);
    return fd;
}


int TCP_server(struct my_info *args, struct net_info *info, int ext_fd, struct socket_list *list, exp_tree **tree) {

    struct addrinfo hints, *res;
    int listen_fd, new_fd, current_fd, interest_fd = 0, errcode, max_fd, count, buffer_id, n;
    fd_set rfds_current, rfds;
    struct sockaddr addr;
    socklen_t addrlen;
    char buffer[128], command[138], buffer_name[128], delim[2] = "\n";
    struct socket_list *aux = NULL;
    struct Cache *local = NULL, *cache = NULL;

    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) return -1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((errcode = getaddrinfo(NULL, args->TCP, &hints, &res)) == -1) return -1;

    if (bind(listen_fd, res->ai_addr, res->ai_addrlen) == -1) return -1;
    if (listen(listen_fd, 5) == -1) return -1;
    freeaddrinfo(res);

    /* if ((buffer = (char *) malloc(BUFFERSIZE)) == NULL) return -1;
     if ((buffer_name = (char *) malloc(BUFFERSIZE)) == NULL) return -1;*/
    if ((local = cache_init(LOCALSIZE)) == NULL) return -1;
    if ((cache = cache_init(CACHESIZE)) == NULL) return -1;

    FD_SET(0, &rfds);
    FD_SET(listen_fd, &rfds);

    if (list != NULL) {
        max_fd = list->fd;
        FD_SET(list->fd, &rfds);
    } else max_fd = 0;

    if (listen_fd > max_fd) max_fd = listen_fd;

    printf("You are now connected to net %d with id %d\n\n", info->net, info->id);

    do {

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

                fgets(command, BUFFERSIZE, stdin); //Get whatever is written in stdin

                if (strncmp(command, "create", 6) == 0) { //Is it a create?

                    ui_create(command, local, info->id);

                } else if (strncmp(command, "get", 3) == 0) {

                    ui_get(command, local, cache, *tree);

                } else if ((strcmp(command, "show topology\n") == 0) || (strcmp(command, "st\n") == 0)) {

                    printf("Extern: %s:%s\n Recovery: %s:%s\n", info->ext_IP,
                           info->ext_TCP, info->rec_IP, info->rec_TCP); //Print Extern and Recovery IPs & TCPs

                } else if (strcmp(command, "show routing\n") == 0 || strcmp(command, "sr\n") == 0) {

                    if (*tree == NULL) printf("Expedition is empty, this node has no friends\n");
                    else print_Tree(*tree); //Print the tree in order, which will show as as ordered array of nodes

                } else if (strcmp(command, "show cache\n") == 0 || strcmp(command, "sc\n") == 0) {

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

                        n = TCP_rcv(aux->fd, command); //Receive message
                        if (n == -1) return -1;//TODO ERROR
                        else if (n == 0) { //Read=0 means the client disconnected

                            printf("Client lost, all nodes connected to socket %d are no longer available\n", aux->fd);
                            if (aux->fd == ext_fd) {
                                if ((strcmp(info->rec_IP, args->IP) != 0) || (strcmp(info->rec_TCP, args->TCP) != 0)) {
                                    printf("Connecting to recovery\n");
                                    strcpy(info->ext_IP, info->rec_IP);
                                    strcpy(info->ext_TCP, info->rec_TCP);
                                    ext_fd = TCP_client(info, list, tree, args);
                                } else {
                                    strcpy(info->ext_IP, args->IP);
                                    strcpy(info->ext_TCP, args->TCP);
                                }
                            }
                            FD_CLR(aux->fd, &rfds);
                            close(aux->fd); //Close the corresponding socket
                            *tree = withdraw_tree(*tree, aux->fd,
                                                  list);//Remove every id whose path contained the socket
                            remove_socket(&list, aux->fd); //Remove the socket from the list
                            break;
                        }
                        if ((strtok(command, delim) != NULL)) {
                            do {
                                if (strncmp(command, "NEW", 3) == 0) {

                                    extern_update(info, args, command);
                                    sprintf(command, "EXTERN %s %s\n", info->ext_IP, info->ext_TCP);
                                    TCP_send(command, aux->fd); //Send Extern message

                                    sprintf(buffer_name, "ADVERTISE %d\n", info->id);
                                    TCP_send(buffer_name, aux->fd);
                                    send_tree(*tree, aux->fd, buffer_id); //Advertise tree to new node

                                } else if (strncmp(command, "ADVERTISE", 9) == 0) {

                                    sscanf(command, "%*s %d", &buffer_id);
                                    *tree = insert(buffer_id, aux->fd, *tree);
                                    sprintf(buffer_name, "ADVERTISE %d\n", buffer_id);
                                    TCP_send_all(buffer_name, list, aux->fd);

                                } else if (strncmp(command, "WITHDRAW", 8) == 0) {

                                    sscanf(command, "%*s %d", &buffer_id);
                                    *tree = del_tree(buffer_id, *tree);
                                    sprintf(buffer_name, "WITHDRAW %d\n", buffer_id);
                                    TCP_send_all(buffer_name, list, aux->fd);

                                } else if (strncmp(command, "INTEREST", 8) == 0) {

                                    sscanf(command, "%*s %s", buffer_name);
                                    if (cache_search(buffer_name, local) >= 0) {

                                        sprintf(command, "DATA %s\n",
                                                buffer_name); //If name was found send back DATA message
                                        TCP_send(command, aux->fd);

                                    } else {
                                        //Continues sending the INTEREST message towards the destination
                                        sscanf(command, "%*s %d", &buffer_id);
                                        current_fd = find_socket(buffer_id, *tree);
                                        interest_fd = current_fd; //Save the fd in which the request came for later
                                        if (current_fd != -1) TCP_send(command, current_fd);
                                        else {
                                            sprintf(command, "NODATA %s\n",
                                                    buffer_name); //If it can´t find the destination, send NODATA
                                            TCP_send(command, aux->fd);
                                        }

                                    }

                                } else if (strncmp(command, "DATA", 4) == 0) {
                                    sscanf(command, "%*s %s", buffer_name);
                                    if (interest_fd != 0) {
                                        cache_add(buffer_name, cache);
                                        TCP_send(command, interest_fd);
                                    } else printf("DATA %s\n", buffer_name);

                                } else if (strncmp(command, "NODATA", 6) == 0) {

                                    sscanf(command, "%*s %s", buffer_name);

                                    if (interest_fd != 0) TCP_send(command, interest_fd);
                                    else printf("NODATA %s\n", buffer_name);

                                }
                            } while ((strtok(NULL, delim)) != NULL);
                        } else printf("No delimeter found, received: %s", command);
                    }
                }
            }
        }
    } while (strcmp(command, "leave\n") != 0);

    erase_tree(*tree);
    close(listen_fd);
    close_list(list);
    // free(buffer);
    // free(buffer_name);
//cache_free(local, LOCALSIZE);
//cache_free(cache, CACHESIZE);
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

void TCP_send_all(char *command, struct socket_list *list, int fd) {

    struct socket_list *aux = list;

    for (; aux != NULL; aux = aux->next) {

        if (fd != aux->fd) TCP_send(command, aux->fd);
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
    buffer[nread] = '\0';
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