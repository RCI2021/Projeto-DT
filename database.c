//
// Created by anton on 30/03/2021.
//

#include "database.h"
#include "client.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include "UDP.h"


enum dbSwitch {
    Local, Cache
};

int data_Init() {

    if ((local = (struct database *) malloc(sizeof(struct database))) == NULL) return -1;
    if ((cache = (struct database *) malloc(sizeof(struct database))) == NULL) return -1;
}

void add_Item(char *new, enum dbSwitch sw) {

    if (sw == Local) {

        if ((local->name[0] == NULL) || (local->last_used == true)) { //TODO local só tem 2 posições ou se é ilimitado?

            local->name[0] = new;
            local->last_used = false;

        } else if ((local->name[1] == NULL) || (local->last_used == false)) {

            local->name[1] = new;
            local->last_used = true;
        }
    }
    if ((cache->name[0] == NULL) || (cache->last_used == true)) {

        cache->name[0] = new;
        cache->last_used = false;

    } else if ((cache->name[1] == NULL) || (cache->last_used == false)) {

        cache->name[1] = new;
        cache->last_used = true;
    }
    return;
}


/*
void rm_Item(char *rm, enum dbSwitch sw) {

    int i;

    if (sw == Local) {


    } else {
        for (i=0;i<DBSIZE;i++){
            if (strcmp(rm,cache->name[i])) free(cache->name[i]);
        }
    }
    return;
}
*/
int get_name(char *search, int id) {

    int id_aux, res, i;

    sscanf(search, "%d.%*s", &id_aux);

    if (id_aux == id) { //TODO

        for (i = 0; i < DBSIZE; i++) {
            if (strcmp(local->name[i], search)) res = i;
        }
    } else {
        if (strcmp(search, cache->name[0])) {
            res = 0;
        } else if (strcmp(search, cache->name[1])) {
            res = 1;
        } else res = -1;
    }

    return res;

}

void show_cache() {
    printf("Cache currently stored:\n1: %s \n2: %s\n", cache->name[0], cache->name[1]);
    return;
}