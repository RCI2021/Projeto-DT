//
// Created by anton on 24/04/2021.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cache.h"
#include "definition.h"


struct Cache *cache_init(int size) {   //TODO free

    int i;
    struct Cache *new;

    new = (struct Cache *) malloc(sizeof(struct Cache));

    if (new == NULL) {
        return NULL;
    }

    if ((new->name = (char **) malloc(sizeof(char *) * size)) == NULL) {
        return NULL;
    }

    for (i = 0; i < size; i++) {
        if ((new->name[i] = (char *) malloc(BUFFERSIZE)) == NULL) break;
        new->name[i][0] = '\0';
    }
    new->last_used = 0;
    new->size = size;
    return new;
}

int cache_add(char *name, struct Cache *cache) {


    strcpy(cache->name[cache->last_used], name);
    if (cache->last_used == (cache->size - 1)) cache->last_used = 0;
    else cache->last_used++;
    return cache->last_used;
}

int cache_rm(char *name, struct Cache *cache) {

    int i;

    for (i = 0; i < cache->size; i++) {

        if (strcmp(cache->name[i], name) == 0) {

            strcpy(cache->name[i], "\0");
            break;
        }
    }
    return i;
}

int cache_search(char *name, struct Cache *cache) {

    int i;

    for (i = 0; i < cache->size; i++) {
        if (strcmp(cache->name[i], name) == 0) return i;
    }
    return -1;
}

void cache_print(struct Cache *cache) {

    int i;

    for (i = 0; i < cache->size; i++) {
        if (cache->name[i][0] == '\000') printf("%d:\t <empty>\n", i);
        else printf("%d:\t %s\n", i, cache->name[i]);
    }


}

void cache_free(struct Cache *cache, int size) {

    int i;

    for (i = 0; i < size; i++) {
        free(cache->name[i]);
    }
    free(cache->name);
    free(cache);
}