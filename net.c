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


int TCP_client(struct net_info *info, struct socket_list *list, exp_tree *tree, struct my_info *my_info) { //RETURN FD

    fd_set rfds;
    struct addrinfo hints, *res;
    int fd, errcode;
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
        TCP_rcv(fd, buffer);
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
    sprintf(buffer, "ADVERTISE %d", info->id);
    TCP_send(buffer, fd);
    return fd;
}


int TCP_server(struct my_info *args, struct net_info *info, struct socket_list *list, exp_tree *tree) {

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

    addrlen = sizeof addr;

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

                addrlen = sizeof addr; //TODO tem que estar aqui?
                if ((new_fd = accept(listen_fd, &addr, &addrlen)) == -1) return -1;//TODO ERROR
                FD_SET(new_fd, &rfds); //Adicionar o novo fd à lista de leitura do select
                if (new_fd > max_fd) max_fd = new_fd; //Atualizar o max_fd

                TCP_rcv(new_fd, buffer); //Recieve NEW message
                list = insertList(list, new_fd); //Add the new fd to the list

                sprintf(buffer, "EXTERN %s %s\n", info->ext_IP, info->ext_TCP); //Create Extern message
                TCP_send(buffer, new_fd); //Send Extern message
                send_tree(tree, new_fd); //Advertise the tree (only does something if there was a failure before

            } else if (FD_ISSET(0, &rfds_current)) { //Does stdin have something for me to read?

                fgets(buffer, BUFFERSIZE, stdin); //Get whatever is written in stdin

                if (strncmp(buffer, "create", 6) == 0) ui_create(buffer, info->id, local);
                else if (strncmp(buffer, "get", 3) == 0) {
                    if (0 < ui_get(buffer, info->id, local) < local->size) {
                    } else if (0 < ui_get(buffer,)) {


                    } else {

                        sprintf(buffer, "INTEREST %s", buffer_name);
                        TCP_send(buffer, find_socket(buffer_id, tree));
                        interest_fd = 0;

                    }

                    }

                } else if ((strcmp(buffer, "show topology\n") == 0) || (strcmp(buffer, "st\n") == 0)) {

                    printf("Extern: %s:%s\n Recovery: %s:%s\n", info->ext_IP, info->ext_TCP, info->rec_IP,
                           info->rec_TCP); //Print Extern and Recovery IPs & TCPs

                } else if (strcmp(buffer, "show routing\n") == 0 || strcmp(buffer, "sr\n") == 0) {
                    print_Tree(tree); //Print the tree in order, which will show as as ordered array of nodes
                } else if (strcmp(buffer, "show cache\n") == 0 || strcmp(buffer, "sc\n") == 0) {

                    if (cache->size == 0) printf("NO CACHE AVAILABLE!"); //Was there a Problem with cache?
                    else {
                        printf("Names stored in Cache:\n"); //Print names stored in cache
                        cache_print(cache);
                        printf("Names stored in Local:\n"); //Print names stored in local database
                        cache_print(local);
                    }


                } else //Command wasn´t recognised, warn the user
                    printf("Unknown Command; Available commands are :\n\tcreate <subname>\n\tget <name>\n\tshow topology\n\tshow routing\n\tshow cache\n\t");

            } else if (list != NULL) { //Are there other sockets connected to me?
                for (aux = list; aux != NULL; aux = aux->next) {    //Which socket has something for me to read?

                    if (FD_ISSET(aux->fd, &rfds_current)) {

                        n = TCP_rcv(aux->fd, buffer); //Receive message
                        if (n == -1) return -1;//TODO ERROR
                        else if (n == 0) { //Read=0 means the client disconnected
                            printf("Client lost, all nodes connected to socket %d are no longer available", aux->fd);
                            close(aux->fd); //Close the corresponding socket
                            remove_socket(list, aux->fd); //Remove the socket from the list
                            tree = withdraw_tree(tree, aux->fd);  //TODO find all ids and send withdraws with one socket
                        } else buffer[n] = '\0'; //Complete the message

                        if (strncmp(buffer, "ADVERTISE", 9) == 0) {

                            sscanf(buffer, "%*s %d", &buffer_id);
                            tree = insert(buffer_id, aux->fd, tree);
                            TCP_send_all(buffer, list, aux->fd);

                        } else if (strncmp(buffer, "WITHDRAW", 8) == 0) {

                            sscanf(buffer, "%*s %d", &buffer_id);
                            delete(buffer_id, tree);
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
                                //If we´re recieving a DATA message, it means that an interest message passed through us before
                                cache_add(buffer_name, cache); //Keep all files coming through us in cache
                                TCP_send(buffer, interest_fd);

                            } else printf("DATA %s\n", buffer_name); //Were we who requested the file?


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

/***********************************************************************************************************************
 * Function that handles sending a message to a socket
 * @param buffer Message to be sent
 * @param fd Socket to send the message
 **********************************************************************************************************************/
void TCP_send(char *buffer, int fd) {

    char *ptr = buffer;
    int nleft, nwritten;
    nleft = strlen(buffer);

    while (nleft > 0) {
        nwritten = write(fd, ptr, nleft);
        nleft -= nwritten;
        ptr += nwritten;
    }
}

/***********************************************************************************************************************
 * Function to handle difusing a message to all of the network
 * @param buffer message to be sent
 * @param list sockets in the net
 * @param fd socket from which the message came, so that it doesn´t send the message backwards
 **********************************************************************************************************************/
void TCP_send_all(char *buffer, struct socket_list *list, int fd) {

    struct socket_list *aux = list;

    for (; aux->next != NULL; aux = aux->next) {

        if (fd != aux->fd) TCP_send(buffer, aux->fd);
    }
}

/***********************************************************************************************************************
 * Function to handle reading a message from a file descriptor
 * @param fd socket to read from
 * @param buffer string to which the message should be written
 * @return number of bytes read
 **********************************************************************************************************************/
int TCP_rcv(int fd, char *buffer) {

    char *ptr = buffer;
    int nread, nleft = BUFFERSIZE;

    while (nleft > 0) {
        nread = read(fd, ptr, nleft);
        if (nread <= 0) break;
        nleft -= nread;
        ptr += nread;
        if (buffer[nread - 1] == '\n') break;
    }

    return nread;
}