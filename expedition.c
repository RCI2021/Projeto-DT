//
// Created by Duds on 20/04/2021.
//
#include <stdlib.h>
#include <stdio.h>

#include "expedition.h"


//inserts node in tree of nodes
link *insert(int id, int fd, link *tree) {

    int h1, h2, h3;
    if (tree == NULL) { //tree is empty
        tree = (link*) malloc(sizeof(link));
        tree->id = id;
        tree->fd = id;
        tree->left = tree->right = NULL;
        return tree;
    }

    if (id == tree->id) return tree; //is already inserted
    if (id < tree->id){
    //inserted in left sub-tree
        tree->left = insert(id, fd, tree->left);
        //checks if tree is balanced
        h1 = height(tree->left->left);
        h2 = height(tree->left->right);
        h3 = height(tree->right);
        if (h1 > h3)
        //rotation to right
            tree = rotate_right(tree);
        if (h2 > h3) {
        //double rotation to right
            tree->left = rotate_left(tree->left);
            tree = rotate_right(tree);
        }
    }

    else {
        //inserted in right sub-tree
        tree->right = insert(id, fd, tree->right);
        h1 = height(tree->right->right);
        h2 = height(tree->right->left);
        h3 = height(tree->left);
        if (h1 > h3)
            tree = rotate_left(tree);   //rotation to left
        if (h2 > h3) {
            tree->right = rotate_right(tree->right);    //double rotation to left
            tree = rotate_left(tree);
        }
    }
    return tree;
}


link *rotate_right(link *tree) {
    link * x, *y, *beta;

    if (tree == NULL) return tree;
    else if (tree->left == NULL) return tree;
    // saves pointers
    y = tree;
    x = tree->left;
    beta = x->right;
    // updates links
    x->right = y;
    y->left = beta;

    return x;
}

link *rotate_left(link *tree) {
    link *x, *y, *beta;

    if (tree == NULL) return tree;
    else if (tree->right == NULL) return tree;
    // saves pointers
    x = tree;
    y = tree->right;
    beta = y->left;
    // updates links
    y->left = x;
    x->right = beta;

    return x;
}

/* checks height of tree */
int height(link *tree) {
    int hl, hr;
    if (tree == NULL) return 0;
    if (tree->left == NULL && tree->right == NULL)
        return 1;
    hl = height(tree->left);
    hr = height(tree->right);
    return ((hl > hr) ? 1 + hl : 1 + hr);
}

//merges two trees (used when deleting node)
link *join(link *a, link *b) {
    if (a == NULL) return b;
    if (b == NULL) return a;
    b = insert(a->id, a->fd, b);
    b = join(a->left, b);
    b = join(a->right, b);
    free(a);
    return b;
}

//deletes node (join avoids losing rest of tree)
link *delete(int id, link *tree) {
    link * aux = tree;
    if (tree == NULL) return NULL;
    if (id < tree->id) tree->left = delete(id, tree->left);
    if (id > tree->id) tree->right = delete(id, tree->right);
    if (id == tree->id) {
        tree = join(tree->left, tree->right);
        free(aux);
    }
    return tree;
}

int find_socket(int id, link *tree) {

    link * aux = tree;
    if (aux == NULL) return -1; //no socket found

    while (aux->left != NULL && aux->right != NULL) {
        if (id < aux->id) aux = aux->left;
        if (id > aux->id) aux = aux->right;
        if (id == aux->id) {
            return aux->fd;
        }
    }

    if (id == aux->id) return aux->fd;
    else return -1;
}

//prints tree in post-fixed
void print_Tree(link *tree) {

    if (tree == NULL) return;
    if (tree->left == NULL) print_Tree(tree->left);
    printf("Node: %d | Socket: %d\n", tree->id, tree->fd);
    if (tree->right == NULL) print_Tree(tree->right);

    return;
}