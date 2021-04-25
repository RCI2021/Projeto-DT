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

};

struct Cache *cache_init(int size) {

    struct Cache *new;
    int i;

    if ((new->name = (char **) malloc(size * sizeof char *))==NULL) return NULL;
    if ((new->last_used = (int *) calloc(sizeof int))==NULL) return NULL;

    for (i = 0; i < size; i++) {
        if ((new->name[i] = (char *) malloc(BUFFERSIZE)) == NULL) return NULL;
    }

    return new;
}

int cache_add(char *name, char **cache,) {


}


void free_cache(char **cache, int size) {

    int i;

    for (i = 0; i < size; i++) {
        free(cache[i]);
    }
    free(cache);
}