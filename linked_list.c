//
// Created by anton on 21/04/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "linked_list.h"

struct socket_list *initLinkedList(void) {
    /* Memory allocation */
    new = (struct socket_list *) malloc(sizeof(struct socket_list));

    return new;
}

struct socket_list *insertList(struct socket_list *next, int fd) {
    struct socket_list *new=initLinkedList();
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
            if (aux->fd > max) max = aux->fd;
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

void remove_socket(struct socket_list **first, int fd) {

    struct socket_list *aux, *prev = NULL;

    aux = *first;

    //Checks if socket is in head node
    if (aux != NULL && aux->fd == fd) {
        *first = aux->next; // Change head
        free(aux);            // free old head
        return;
    }

        // Else Search for the key to be deleted, keep track of the previous node as we, need to change 'prev->next'
    else {
        while (aux != NULL && aux->fd != fd) {
            prev = aux;
            aux = aux->next;
        }

        // If key was not present in linked list
        if (aux == NULL)
            return;

        // Unlink the node from linked list
        prev->next = aux->next;

        // Free memory
        free(aux);
    }
}

void close_list(struct socket_list *list) {
    struct socket_list *aux;
    for (aux = list; aux != NULL; aux = aux->next) {
        close(aux->fd);
    }
    freeList(list);
    return;
}