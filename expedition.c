//
// Created by Duds on 20/04/2021.
//
#include <stdlib.h>
#include <stdio.h>

#include "expedition.h"
#include "net.h"


//inserts node in tree of nodes
exp_tree *insert(int id, int fd, exp_tree *tree) {

    int h1, h2, h3;
    if (tree == NULL) { //tree is empty
        tree = (exp_tree *) malloc(sizeof(exp_tree));
        tree->id = id;
        tree->fd = fd;
        tree->left = tree->right = NULL;
        return tree;
    }

    if (id == tree->id) {
        tree->fd = fd;
        return tree; //switch for new fd
    }

    if (id < tree->id) {
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
    } else {
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


exp_tree *rotate_right(exp_tree *tree) {
    exp_tree *x, *y, *beta;

    if (tree == NULL) return tree;
    else if (tree->left == NULL) return tree;
    // saves pointers
    y = tree;
    x = tree->left;
    beta = x->right;
    // updates exp_trees
    x->right = y;
    y->left = beta;

    return x;
}

exp_tree *rotate_left(exp_tree *tree) {
    exp_tree *x, *y, *beta;

    if (tree == NULL) return tree;
    else if (tree->right == NULL) return tree;
    // saves pointers
    x = tree;
    y = tree->right;
    beta = y->left;
    // updates exp_trees
    y->left = x;
    x->right = beta;

    return y;
}

/* checks height of tree */
int height(exp_tree *tree) {
    int hl, hr;
    if (tree == NULL) return 0;
    if (tree->left == NULL && tree->right == NULL)
        return 1;
    hl = height(tree->left);
    hr = height(tree->right);
    return ((hl > hr) ? 1 + hl : 1 + hr);
}

//merges two trees (used when deleting node)
exp_tree *merge(exp_tree *a, exp_tree *b) {
    if (a == NULL) return b;
    if (b == NULL) return a;
    b = insert(a->id, a->fd, b);
    b = merge(a->left, b);
    b = merge(a->right, b);
    free(a);
    return b;
}

//deletes node (merge avoids losing rest of tree)
exp_tree *del_tree(int id, exp_tree *tree) {
    exp_tree *aux = tree;
    if (tree == NULL) return NULL;
    if (id < tree->id) tree->left = del_tree(id, tree->left);
    if (id > tree->id) tree->right = del_tree(id, tree->right);
    if (id == tree->id) {
        aux = merge(tree->left, tree->right);
        free(tree);
    }
    return aux;
}

int find_socket(int id, exp_tree *tree) {

    exp_tree *aux = tree;
    if (aux == NULL) return -1; //no socket found

    while (aux->left != NULL || aux->right != NULL) {
        if (id < aux->id) aux = aux->left;
        else if (id > aux->id) aux = aux->right;
        else if (id == aux->id) {
            return aux->fd;
        }
    }

    if (id == aux->id) return aux->fd;
    else return -1;
}

//prints tree in-fixed
void print_Tree(exp_tree *tree) {

    if (tree == NULL) return;
    if (tree->left != NULL) print_Tree(tree->left);
    printf("Node: %d | Socket: %d\n", tree->id, tree->fd);
    if (tree->right != NULL) print_Tree(tree->right);

    return;
}

exp_tree *send_tree(exp_tree *tree, int fd, int id) {

    char buffer[BUFFERSIZE];

    if (tree == NULL) return NULL;

    if (tree->left != NULL) tree->left = send_tree(tree->left, fd, id);
    if (tree->id != id) {
        sprintf(buffer, "ADVERTISE %d\n", tree->id);
        TCP_send(buffer, fd);
    }
    if (tree->right != NULL) tree->right = send_tree(tree->right, fd, id);

    return tree;
}

exp_tree *withdraw_tree(exp_tree *tree, int fd, struct socket_list *list) {

    char buffer[BUFFERSIZE];

    if (tree == NULL) return NULL;
    if (tree->left != NULL)tree->left = withdraw_tree(tree->left, fd, list);
    if (tree->right != NULL)tree->right = withdraw_tree(tree->right, fd, list);

    if (tree->fd == fd) {
        sprintf(buffer, "WITHDRAW %d\n", tree->id);
        TCP_send_all(buffer, list, fd);
        tree = del_tree(tree->id, tree);
    }
    return tree;
}

exp_tree *erase_tree(exp_tree *tree) {

    if (tree == NULL) return NULL;
    if (tree->left != NULL) erase_tree(tree->left);
    if (tree->right != NULL) erase_tree(tree->right);

    free(tree);
    return NULL;
}
