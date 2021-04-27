//
// Created by anton on 18/04/2021.
//

#ifndef PROJETO_DT_USER_INTERFACE_H
#define PROJETO_DT_USER_INTERFACE_H

#define CMDSIZE 128

#include "definition.h"

enum state_main {

    wait, get_nodeslist, join, connected, err, quit

};

int arg_verify(struct my_info *args, int argc, char **argv); //Argument Verification

enum state_main command_handle(char *command, struct my_info *args, struct net_info *info); //Command Switcher

int ui_create(char *buffer, int id, struct Cache *local);

int ui_get(char *buffer, int id, struct Cache *data);

#endif //PROJETO_DT_USER_INTERFACE_H
