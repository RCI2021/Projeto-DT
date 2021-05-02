//
// Created by anton on 18/04/2021.
//

#ifndef PROJETO_DT_USER_INTERFACE_H
#define PROJETO_DT_USER_INTERFACE_H

#define CMDSIZE 128

#include "definition.h"
#include "cache.h"
#include "expedition.h"

enum state_main {

    wait, get_nodeslist, join, connected, err, quit

};

int arg_verify(struct my_info *args, int argc, char **argv); //Argument Verification
enum state_main command_handle(char *command, struct my_info *args, struct net_info *info); //Command Switcher
void ui_create(char *buffer, struct Cache *local, int id);

struct interest_list *
ui_get(char *buffer, struct Cache **local, struct Cache **cache, struct interest_list *interestList, exp_tree *tree,
       int my_id);

#endif //PROJETO_DT_USER_INTERFACE_H
