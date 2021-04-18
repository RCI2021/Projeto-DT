//
// Created by Duds on 11/04/2021.
//

#include "net_interface.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "registration.h" //todo
#include "database.h"

void rcv_advertise(struct net_info *info, char *buffer, struct node *head_fd) {
    //TODO adicionar o id à lista do nó e enviar para os nós seguintes (atenção p/ n repetir o nò!)
}