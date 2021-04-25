//
// Created by anton on 24/04/2021.
//
#include <stdlib.h>
#include "cache.h"
#include "definition.h"

struct Cache {

    char **name;
    //FP somethin
    int last_used;
    int size;

};

int cache_init(struct Cache *new, int size) {

    int i;

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

            //TODO change

        }
    }

    for (i = 0; i < cache->size; i++) {
        if (cache->last_used != i) {

            //TODO change

        }
    }
}

int cache_rm(char *name, struct Cache *cache) {

    int i;

    for (i = 0; i < cache->size; i++) {

        if (strcmp(cache->name[i], name) == 0) {

            //TODO remove name

        }
    }
}

int cache_search(char *name, struct Cache *cache) {

    int i;

    for (i = 0; i < cache->size; i++) {

        if (strcmp(cache->name[i], name) == 0) {

            //TODO return position name

        }
    }
//return nodata
}


void cache_free(char **cache, int size) {

    int i;

    for (i = 0; i < size; i++) {
        free(cache[i]);
    }
    free(cache);
}