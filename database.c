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
#include "registration.h"


enum dbSwitch {
    Local, Cache
};

int data_Init(struct database *local, struct database *cache) {

    if ((local = (struct database *) malloc(sizeof(struct database))) == NULL) return -1;
    if ((cache = (struct database *) malloc(sizeof(struct database))) == NULL) return -1;

    return 0;
}

void add_Item(char *new, struct database *db) {

struct database *aux;
    for (aux=db;aux->next==NULL;aux=aux->next);



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
