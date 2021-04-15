//
// Created by anton on 01/04/2021.
//

#ifndef PROJETO_DT_EXPEDITION_H
#define PROJETO_DT_EXPEDITION_H

struct socket_list {
    int fd;         //file descriptor of socket
    Node *node;        //list of nodes associated with socket
    socket_list *next;      //pointer to next socket
};

struct Node {
    int id;
    Node *next;
};


#endif //PROJETO_DT_EXPEDITION_H
