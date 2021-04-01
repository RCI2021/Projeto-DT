//
// Created by anton on 01/04/2021.
//
#include <stdbool.h>
#include "expedition.h"

struct socket_list new_socket(int fd) {

    struct socket_list *new_list;
    struct Node *new_node;
    bool err;

    if (new_list = (struct socket_list *) malloc(sizeof(struct socket_list)) == NULL) err = true;
    if (new_node = (struct Node *) malloc(sizeof(struct Node)) == NULL) err = true;

    new_node->fd = fd;
    new_list->node = new_node;
    new_list->count++;

    if (err == true) return NULL;
    else return new_list;
}

enum bool add_node(struct socket_list *list, int fd) {

    struct Node *aux, *new;
    bool err;

    if ((new = (struct Node *) malloc(sizeof(struct Node))) == NULL) err = true;

    new->fd = fd;
    aux = list->node;
    while (aux->next != NULL) aux = aux->next;

    aux->next = new;
    list->count++;

    return err;
}

enum bool rm_node(struct socket_list *list, int fd) {


}