//
// Created by Duds on 20/04/2021.
//

#ifndef PROJETO_DT_EXPEDITION_H
#define PROJETO_DT_EXPEDITION_H

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

exp_tree *delete(
int id, exp_tree
*tree);

int find_socket(int id, exp_tree *tree);

void print_Tree(exp_tree * tree);

exp_tree *send_tree(exp_tree * tree, int
fd);

#endif //PROJETO_DT_EXPEDITION_H
