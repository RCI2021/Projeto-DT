//
// Created by anton on 01/04/2021.
//

#ifndef PROJETO_DT_EXPEDITION_H
#define PROJETO_DT_EXPEDITION_H

struct socket_list {
    int count;
    Node *node;
    socket_list *next;
};

struct Node {
    int fd;
    Node *next;
};


#endif //PROJETO_DT_EXPEDITION_H
