//
// Created by anton on 21/04/2021.
//

#ifndef PROJETO_DT_LINKED_LIST_H
#define PROJETO_DT_LINKED_LIST_H

typedef struct socket_list_struct socket_list;

struct socket_list_struct {
    int fd;
    socket_list *next;
};

socket_list *initLinkedList(void);

socket_list *insertList(socket_list *next, int fd);

void freeList(socket_list *first);

int getSocket(socket_list *node);

socket_list *getNextSocket(socket_list *node);


#endif //PROJETO_DT_LINKED_LIST_H
