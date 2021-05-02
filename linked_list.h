//
// Created by anton on 21/04/2021.
//

#ifndef PROJETO_DT_LINKED_LIST_H
#define PROJETO_DT_LINKED_LIST_H

struct socket_list {
    int fd;
    struct socket_list *next;
};

struct interest_list {

    int fd;
    char name[128];
    struct interest_list *next;

};

struct socket_list *insertList(struct socket_list *next, int fd);

void freeList(struct socket_list **first);

int getSocket(struct socket_list *node);


int FD_setlist(struct socket_list *list, fd_set rfds);

struct socket_list *getNextSocket(struct socket_list *node);

struct socket_list *remove_socket(struct socket_list *first, int fd);

void close_list(struct socket_list *list);

void printList(struct socket_list *lpointer);

struct interest_list *insertInterest(struct interest_list *next, int fd, char *name);

int interest_search(struct interest_list *list, char *name);

void interest_rm(struct interest_list **list, char *name);

#endif //PROJETO_DT_LINKED_LIST_H
