//
// Created by anton on 05/04/2021.
//

#include "client.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>


int FD_setlist(struct node *head_fd, fd_set *rfds) {    //Todo ideia do tiago de copiar o rfds

    struct node *aux = head_fd;
    int max;

    FD_ZERO(&rfds);

    while (aux->next != NULL) {

        FD_SET(aux->fd, &rfds);
        if (max < aux->fd) max = aux->fd;
        aux = aux->next;

    }

    return max;
}

int add_item_list(int fd, struct node *head) {

    struct node *aux, *new;
    aux = head;

    while (aux->next != NULL) aux = aux->next;

    if ((new = (struct node *) malloc(sizeof(struct node))) == NULL) return -1;

    new->fd = fd;
    aux->next = new;

    return 0;
}

void rm_item_list(int fd, struct node *head) {

    struct node *aux, *next;

    while (aux->next->fd != fd) aux->next;

    next = aux->next->next;
    free(aux->next->fd);
    free(aux->next);
    aux->next = next;

}

struct node *create_list() {

    struct node *head;

    if ((head = (struct node *) malloc(sizeof(struct node))) == NULL) return NULL;

    return head;

}

int max_fd_list(struct node *head) {

    int max;
    struct node *aux = head;


    while (aux != NULL) {
        if (aux->fd > max)max = aux->fd;
        aux = aux->next;
    }

    return max + 1;
}

void free_list(struct node *head) {

    struct node *aux, *next;

    for (aux = head; aux != NULL, aux = next) {
        next = aux->next;
        free(aux->fd);
        free(aux);
    }
}


int get_next_list(struct node *current) {

    return ((current->next == NULL) ? NULL : current->next->fd);

}