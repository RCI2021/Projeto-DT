//
// Created by anton on 24/04/2021.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cache.h"
#include "definition.h"



int cache_init(struct Cache *new, int size) {   //TODO free

    int i;

    new = (struct Cache *) malloc(sizeof (struct Cache));

    if(new == NULL){
        return -1;
    }

    if ((new->name = (char **) malloc(sizeof (char*) * size)) == NULL) {
        return -1;
    }

    for (i = 0; i < size; i++) {
        if ((new->name[i] = (char *) malloc(BUFFERSIZE)) == NULL) break;
    }
    new->last_used = -1;
    new->size = size;
    return i;
}

int cache_add(char *name, struct Cache *cache) {

    int i;

    for (i = 0; i < cache->size; i++) {
        if (cache->name[i] == NULL) {

            strcpy(cache->name[i], name);
            return i;
        }
    }

    for (i = 0; i < cache->size; i++) {
        if (cache->last_used != i) {

            strcpy(cache->name[i], name);
            break;

        }
    }
    return i;
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

    for (i = 0; i < cache->size; i++) printf("%d:\t %s", i, cache->name[i]);

}

void cache_free(char **cache, int size) {

    int i;

    for (i = 0; i < size; i++) {
        free(cache[i]);
    }
    free(cache);
}