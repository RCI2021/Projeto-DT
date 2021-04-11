//
// Created by anton on 30/03/2021.
//

#include "database.h"
#include "client.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UDP.h"

enum dbSwitch {
    Local, Cache
};

static struct database *local, *cache;

int data_Init() {

    if ((local = (struct database *) malloc(sizeof(struct database))) == NULL) return -1;
    if ((cache = (struct database *) malloc(sizeof(struct database))) == NULL) return -1;

    return 0;
}

void add_Item(char *new, enum dbSwitch sw) {

    struct f_list *aux;
    int i;

    if (sw == Local) aux = local->list;
    local->size++;

    else aux = cache->list;
    cache->size++;

    if (sw != Cache || ((cache->size <= CACHESIZE) && (sw == Cache))) {
        for (; aux->next != NULL; aux = aux->next);
        aux->next = create_item(new);

    } else {
        for (i = 0; i < cache->cache_last_used; i++)aux = aux->next;
        aux->name = new;
    }

    return;
}

void rm_Item(char *name) {

    struct f_list *aux = local->list, *rm;

    if (!strcmp(aux->name, name)) {

        for (; aux->next != NULL; aux = aux->next) {

            if (strcmp(aux->next->name, name)) break;

        }
        rm = aux->next;
        aux->next = aux->next->next;

    } else {
        rm = aux;
        aux = aux->next;
    }

    free(rm->name);
    free(rm);
    return;

}

int get_file(char *name, enum dbSwitch *sw) {

    struct f_list *aux;
    int i;
    if (sw == Local) aux = local->list;
    else aux = cache->list;

    for (i = 0; aux->next != NULL; i++) {

        if (strcmp(name, aux->name))break;

    }//TODO

    return i;

}

struct f_list *create_item(char *name) {
    struct f_list *new;
    if ((new = (struct f_list *) malloc(sizeof(struct f_list))) == NULL) return NULL;
    new->name = name;
    return new;

}