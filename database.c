//
// Created by anton on 30/03/2021.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include "registration.h"
#include "database.h"
#include "client.h"


enum dbSwitch {
    Local, Cache
};

int data_Init(struct database *local, struct database *cache) {

    if ((local = (struct database *) malloc(sizeof(struct database))) == NULL) return -1;
    if ((cache = (struct database *) malloc(sizeof(struct database))) == NULL) return -1;

    return 0;
}

int add_Item(char *name, struct database *db, enum dbSwitch *state) {

    struct f_list *new, *aux;
    int i;

    if (state == Local) i = DBSIZE;
    else i = CACHESIZE;

    for (aux = db->list; i > 0 || aux->next == NULL; i--, aux = aux->next) {

        if (strcmp(name, aux->name)) {

            perror("File already exists");
            return -1;
        }
    }

    if ((new = (struct f_list *) malloc(sizeof(struct f_list)) == NULL)) return 137;
    new->name = name;

    new->next = NULL;
    aux->next = new;

    return 0;
}

void rm_Item(char *name, struct database *db) {

    struct f_list *rm, *aux;
    int i = DBSIZE;

    for (aux = db->list; i > 0 || aux->next == NULL; i--, aux = aux->next) {

        if (strcmp(name, aux->next->name)) {

            rm = aux->next;
            aux->next = rm->next;
            free(rm);
            break;
        }
    }
}

void sub_Item(char *name, struct database *db) {

    struct f_list *last_used, *aux;
    int i;

    for (aux = db->list, i = 0; aux->next == NULL; aux = aux->next, i++) {

        if (strcmp(aux->name, name)) {

            perror("File already exists");
            return;
        }

        if (i == aux->cache_last_used) last_used = aux;
    }

    last_used->name = name;
    return;
}

int search_Item(char *name, struct database *db) {

    struct f_list *aux = db->list;
    int i;

    for (i = 0; aux->next == NULL; aux = aux->next, i++) {  //Run the list

        if (strcmp(aux->name, name)) return i;  //Compare with required name

    }

    return -1;

}
