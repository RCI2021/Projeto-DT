//
// Created by anton on 05/04/2021.
//

#ifndef PROJETO_DT_LIST_H
#define PROJETO_DT_LIST_H


struct socket_list {
    int fd;
    //todo lista de nós
    struct socket_list *next
};


int FD_setlist(struct socket_list *head_fd, fd_set *rfds);

int add_item_list(int fd, struct socket_list *head);

int add_item_list(int fd, struct socket_list *head);

int get_next_list(struct socket_list *current);

void free_list(struct socket_list *head);

struct socket_list *create_list();

int max_fd_list(struct socket_list *head);

#endif //PROJETO_DT_LIST_H
