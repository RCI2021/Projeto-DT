//
// Created by anton on 30/03/2021.
//

#include "database.h"
#include "client.h"

struct database *local, *cache;
enum dbSwitch {
    Local, Cache
};

int data_Init(int size) {

    if (local = (struct database *) malloc(sizeof(struct database)) == NULL) return -1;
    if (cache = (struct database *) malloc(sizeof(struct database)) == NULL) return -1;
}

void add_Item(char *new, enum dbSwitch sw) {

    if (sw == Local) {

        if ((local->name[0] == NULL) || (local->last_used == 1)) {

            local->name[0] = new;
            local->last_used = 0;

        } else if ((local->name[1] == NULL) || (local->last_used == 0)) {

            local->name[1] = new;
            local->last_used = 1;
        }
    } else {

        if ((cache->name[0] == NULL) || (cache->last_used == 1)) {

            cache->name[0] = new;
            cache->last_used = 0;

        } else if ((cache->name[1] == NULL) || (cache->last_used == 0)) {

            cache->name[1] = new;
            cache->last_used = 1;
        }
    }
    return;
}

void rm_Item(char *rm, enum dbSwitch sw) {

    if (sw == Local) {

        if (strcmp(rm, local->name[0]) == 0) {

            free(local->name[0]);
            local->last_used = 1;

        } else if (strcmp(rm, local->name[1]) == 0) {

            free(local->name[1]);
            local->last_used = 0;
        }
    } else {

        if (strcmp(rm, cache->name[0]) == 0) {

            free(cache->name[0]);
            cache->last_used = 1;

        } else if (strcmp(rm, cache->name[1]) == 0) {

            free(cache->name[1]);
            cache->last_used = 0;
        }
    }
    return;
}

int get_name(char *search, int id) {

    int *id_aux, res;
    char *name_aux;

    if ((id_aux = (int *) malloc(sizeof(int))) == NULL) return -137;
    if ((name_aux = (char *) malloc(sizeof(char) * MAXNAMESIZE)) == NULL) return -137;

    sscanf(search, "%d.%s", &id_aux, name_aux);

    if (id_aux == MYID/*TODO*/) {

        if strcmp(name_aux, local->name[0])
        {
            res = 0;
        } else if (strcmp(name_aux, local->name[1])) {
            res = 1;
        } else res = -1

    } else {
        if strcmp(name_aux, cache->name[0])
        {
            res = 0;
        } else if (strcmp(name_aux, cache->name[1])) {
            res = 1;
        } else res = -1
    }

    free(id_aux):
    free(name_aux);
    return res;

}

void show_cache() {
    printf("Cache currently stored:\n %s \n %s\nThe last Item used was: %d\n", cache->name[0], cache->name[1],
           cache->last_used);
    return;
}