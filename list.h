//
// Created by anton on 05/04/2021.
//

#ifndef PROJETO_DT_LIST_H
#define PROJETO_DT_LIST_H


struct node {
    int fd;
    struct node *next
};


int FD_setlist(struct node *head_fd, fd_set *rfds);

int add_item_list(int fd, struct node *head);

int add_item_list(int fd, struct node *head);

int get_next_list(struct node *current);

void free_list(struct node *head);

struct node *create_list(int fd);

int max_fd_list(struct node *head);

#endif //PROJETO_DT_LIST_H
