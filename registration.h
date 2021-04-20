//
// Created by anton on 18/04/2021.
//

#ifndef PROJETO_DT_REGISTRATION_H
#define PROJETO_DT_REGISTRATION_H
#include "definition.h"

int reg(struct my_info *args, struct net_info *info); //Register Node on UDP server
int nodeslist(struct my_info *args, struct net_info *info);

int unreg(struct my_info *args, struct net_info *info);

int UDP_exch(char *message, char *buffer, struct my_info *args); //Send UDP message and retrieve response

#endif //PROJETO_DT_REGISTRATION_H
