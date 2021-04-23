//
// Created by anton on 21/04/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "linked_list.h"

struct socket_list *initLinkedList(void) {
    return NULL;
}

struct socket_list *insertList(struct socket_list *next, int fd) {
    struct socket_list *new;

    /* Memory allocation */
    new = (struct socket_list *) malloc(sizeof(struct socket_list));

    /* Check memory allocation errors */
    if (new == NULL)
        return NULL;

    /* Initialize new node */
    new->fd = fd;
    new->next = next;

    return new;
}

int FD_setlist(struct socket_list *list, fd_set *rfds) {

    struct socket_list *aux = list;
    int max = 0;
    if (aux != NULL) {
        for (; aux->next != NULL; aux = aux->next) {

            FD_SET(aux->fd, rfds);
            if (aux->fd > max)max = aux->fd;
        }
    }
    return max;

}

void freeList(struct socket_list *first) {
    struct socket_list *aux, *next;

    /* Cycle from the first to the last element           */
    for (aux = first; aux != NULL; aux = next) {
        next = aux->next;           /* Keep track of the next node */
        free(aux);                  /* Free current node    */
    }
}

int getSocket(struct socket_list *node) {
    if (node == NULL)  /* Check that node is not empty */
        return -1;

    return node->fd;
}

struct socket_list *getNextSocket(struct socket_list *node) {
    return ((node == NULL) ? NULL : node->next);
}