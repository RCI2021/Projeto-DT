//
// Created by anton on 01/04/2021.
//
#include <stdbool.h>
#include "expedition.h"

typedef struct node link;

struct node {
    int id, fd;
    link *left;
    link *right;
}

/*//Adds a new node
link * new_node(int id, link *left, link *r) {
    link *x = (link *) malloc(sizeof(link);
    x->id = id;
    x->left = left;
    x->right = right;
    return x;
}

//Verifies max number
link *max(int *id, int left, int right) {
    int m = (left + right) / 2;
    int u, v;
    link *x = new_node(id[m], NULL, NULL);

    if (left == right) return x;
    x->left = max(id, left, m);
    x->right = max(id, m + 1, right);
    u = x->left->id;
    v = x->right->id;
    if (u > v)
        x->id = u;
    else x->id = v;
    return x;
}*/

link
*

insert(int id, int fd, link *tree) {
    link * upper, *child, *leaf;

    upper = tree;
    child = tree;

    if (tree == NULL) //creates tree
    {
        tree = (link *) malloc(sizeof(link));
        tree->id = id;
        tree->fd = fd;
        tree->left = tree->right = NULL;  //set empty sub-trees
        return tree;
    }

    //goes down the tree. if id is bigger goes right if smaller goes left
    while (child != NULL) {
        upper = child;
        if (child->id == id) return tree;
        child = (child->id > id) ? child->left : child->right;
    }

//creates new leaf
    leaf = (link *) malloc(sizeof(link));
    leaf->data = id;
    leaf->left = leaf->right = NULL;
    /* upper passa a nó intermédio e aponta para nova folha */
    if (upper->data > id) upper->left = leaf;
    else upper->right = leaf;
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

link *join(link *a, link *b) {
    if (a == NULL) return b;
    if (b == NULL) return a;
    b = insert(a->id, b);
    b = join(a->left, b);
    b = join(a->right, b);
    free(a);
    return b;
}

//TODO something fishy
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

    int sckt;

    link * aux = tree;
    if (aux == NULL) return -1; //no socket found

    while (aux->left != NULL && aux->right != NULL) {
        if (id < aux->id) aux = aux->left;
        if (id > aux->id) aux = aux->right;
        if (id == aux->id) {
            return sckt;
        }
    }

    if (id == aux->id) return sckt;
    else return -1;
}

void print_Tree(link *tree) {

    if (tree == NULL) return;
    if (tree->left == NULL) print_Tree(tree->left);
    if (tree->right == NULL) print_Tree(tree->right);
    //TODO fazer isto quando n estiver cheio de fome (recursividade para imprimir nós em in-fixado)
    printf("Node: %d | Socket: %d", tree->id, tree->fd);

    return;
}