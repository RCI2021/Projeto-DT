//
// Created by anton on 21/04/2021.
//
#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

socket_list *initLinkedList(void) {
    return NULL;
}

socket_list *insertList(socket_list *next, int fd) {
    socket_list *new;

    /* Memory allocation */
    new = (socket_list *) malloc(sizeof(socket_list));

    /* Check memory allocation errors */
    if (new == NULL)
        return NULL;

    /* Initialize new node */
    new->this = this;
    new->next = next;

    return new;
}

void freeList(socket_list *first) {
    socket_list *aux, *next;

    /* Cycle from the first to the last element           */
    for (aux = first; aux != NULL; aux = next) {
        next = aux->next;           /* Keep track of the next node */
        free(aux);                  /* Free current node    */
    }
    return;
}

int getSocket(socket_list *node) {
    if (node == NULL)  /* Check that node is not empty */
        return NULL;

    return node->fd;
}

socket_list *getNextSocket(socket_list *node) {
    return ((node == NULL) ? NULL : node->next);
}