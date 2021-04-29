//
// Created by Duds on 20/04/2021.
//

#ifndef PROJETO_DT_EXPEDITION_H
#define PROJETO_DT_EXPEDITION_H

#include "linked_list.h"

typedef struct node exp_tree;

struct node {
    int id, fd;
    exp_tree *left;
    exp_tree *right;
};

exp_tree *insert(int id, int fd, exp_tree *tree);

exp_tree *rotate_right(exp_tree *tree);

exp_tree *rotate_left(exp_tree *tree);

int height(exp_tree *tree);

exp_tree *merge(exp_tree *a, exp_tree *b);

exp_tree *del_tree(int id, exp_tree *tree);

int find_socket(int id, exp_tree *tree);

void print_Tree(exp_tree * tree);

exp_tree *send_tree(exp_tree * tree, int fd);

exp_tree *withdraw_tree(exp_tree *tree, int fd, struct socket_list *list);

void erase_tree(exp_tree *tree);

#endif //PROJETO_DT_EXPEDITION_H
