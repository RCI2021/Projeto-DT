//
// Created by anton on 21/04/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include "linked_list.h"

struct socket_list *insertList(struct socket_list *next, int fd) {

    struct socket_list *new = NULL;

    new = (struct socket_list *) malloc(sizeof(struct socket_list));
    /* Check memory allocation errors */
    if (new == NULL)
        return NULL;

    /* Initialize new node */
    new->fd = fd;
    new->next = next;

    return new;
}

int FD_setlist(struct socket_list *list, fd_set rfds) {

    struct socket_list *aux = list;
    int max = 0;
    if (aux != NULL) {
        for (; aux->next != NULL; aux = aux->next) {

            FD_SET(aux->fd, &rfds);
            if (aux->fd > max) max = aux->fd;
        }
    }
    return max;

}

void freeList(struct socket_list *first) {
    struct socket_list *aux, *next;

    if (first != NULL) {
        /* Cycle from the first to the last element           */
        for (aux = first; aux != NULL; aux = next) {
            next = aux->next;           /* Keep track of the next node */
            free(aux);                  /* Free current node    */
        }
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

void printList(struct socket_list *lpointer) {
    struct socket_list *aux;
    int i;

    for (aux = lpointer, i = 0; aux != NULL; aux = aux->next, i++) {
        printf("%d (%d)| ", aux->fd, i);
    }
    printf("\n");
}

struct interest_list *insertInterest(struct interest_list *next, int fd, char *name) {

    struct interest_list *new = NULL;

    new = (struct interest_list *) malloc(sizeof(struct interest_list));
    /* Check memory allocation errors */
    if (new == NULL)
        return NULL;

    /* Initialize new node */
    new->fd = fd;
    new->next = next;
    strcpy(new->name, name);

    return new;
}

int interest_search(struct interest_list *list, char *name) {

    struct interest_list *aux;

    for (aux = list; aux != NULL; aux = aux->next) {
        if (strcmp(name, aux->name) == 0) {     //checks if name is correct
            return aux->fd;
        }
    }
    return 0;
}

void interest_rm(struct interest_list **list, char *name) {

    struct interest_list *aux = *list, *prev;
    if ((aux != NULL) && (strcmp(aux->name, name) == 0)) {

        *list = aux->next;
        free(aux);
        return;

    }

    while (aux != NULL && (strcmp(aux->name, name) == 0)) {

        prev = aux;
        aux = aux->next;

    }

    if (aux == NULL) return;

    prev->next = aux->next;

    free(aux);
}