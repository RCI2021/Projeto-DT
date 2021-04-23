//
// Created by anton on 21/04/2021.
//

#ifndef PROJETO_DT_LINKED_LIST_H
#define PROJETO_DT_LINKED_LIST_H

struct socket_list {
    int fd;
    struct socket_list *next;
};

struct socket_list *initLinkedList(void);

struct socket_list *insertList(struct socket_list *next, int fd);

void freeList(struct socket_list *first);

int getSocket(struct socket_list *node);


int FD_setlist(struct socket_list *list, fd_set *rfds);

struct socket_list *getNextSocket(struct socket_list *node);


#endif //PROJETO_DT_LINKED_LIST_H
