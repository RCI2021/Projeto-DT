//
// Created by Duds on 20/04/2021.
//

#ifndef PROJETO_DT_EXPEDITION_H
#define PROJETO_DT_EXPEDITION_H

typedef  struct node link;

struct node {
    int id, fd;
    link *left;
    link *right;
};

link *insert(int id, int fd, link *tree);

link *rotate_right(link *tree);

link *rotate_left(link *tree);

int height(link *tree);

link *join(link *a, link *b);

link *delete(int id, link *tree);

int find_socket(int id, link *tree);

void print_Tree(link *tree);


#endif //PROJETO_DT_EXPEDITION_H
