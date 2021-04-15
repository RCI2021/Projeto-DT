//
// Created by anton on 15/04/2021.
//

#ifndef PROJETO_DT_USER_INTERFACE_H
#define PROJETO_DT_USER_INTERFACE_H

enum state_main {

    wait, join_UDP, connected, quit

};

//Required by main.c
enum state_main command_handle(char *command, struct net_info *info);    //Handle commands given by the user
int arg_verify(struct my_info *args, int argc, char **argv); //Argument Verification
#endif //PROJETO_DT_USER_INTERFACE_H
