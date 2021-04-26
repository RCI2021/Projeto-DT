//
// Created by anton on 24/04/2021.
//

#ifndef PROJETO_DT_CACHE_H
#define PROJETO_DT_CACHE_H

struct Cache {

    char **name;
    //FP somethin
    int last_used;
    int size;

};

int cache_init(struct Cache *new, int size);

int cache_add(char *name, struct Cache *cache);

int cache_rm(char *name, struct Cache *cache);

int cache_search(char *name, struct Cache *cache);

void cache_print(struct Cache *cache);

void cache_free(char **cache, int size);

#endif //PROJETO_DT_CACHE_H
