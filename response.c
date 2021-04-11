//
// Created by Duds on 11/04/2021.
//

#include "response.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "UDP.h"

int create(char *name) {
}

void show_topology(struct net_info *info){

    printf("Extern: %s:%s\n Recovery: %s:%s\n",info->ext_IP,info->ext_TCP,info->rec_IP,info->rec_TCP);
    return;
}

void show_

void extern(struct struct net_info *info, char* buffer)
{

}

void advertise(struct net_info *info, char *buffer, struct node *head_fd) {

    //TODO adicionar o id à lista do nó e enviar para os nós seguintes (atenção p/ n repetir o nó!)
}