
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_interface.h"


//Function Definitions
int info_alloc(struct my_info *args, struct net_info *info, char *command); //Memory Allocation
void info_free(struct my_info *args, struct net_info *info, char *command); //Memory "Deallocation"



int main(int argc, char **argv) {

    struct my_info *args = NULL;    //Info about myself
    struct net_info *info = NULL;   //Info about the net I´m in
    char *command;  //Command Buffer
    enum state_main state = wait;   //State switch

    if (info_alloc(args, info, command) != 0) return -1; //Memory allocation
    if (arg_verify(args, argc, argv) != 0) return -1;   //Argument Verification

    do {
        switch (state) {
            case wait:
                fgets(command, CMDSIZE, stdin);
                state = command_handle(command, info);
                break;
            case join:

        }

    } while (state != quit)

}

/***********************************************************************************************************************
 * Function to allocate memory required throughout the program
 * @param args struct containing information about the node
 * @param info struct containing information about the net connecting
 * @param command buffer used when reading commands from stdin
 * @return 0 if OK, 1 else
 **********************************************************************************************************************/
int info_alloc(struct my_info *args, struct net_info *info, char *command) {

    if ((args = (struct my_info *) malloc(sizeof(struct my_info))) == NULL) return 1;
    if ((args->IP = (char *) malloc(IPSIZE)) == NULL) return 1;
    if ((args->TCP = (char *) malloc(TCPSIZE)) == NULL) return 1;
    if ((args->regIP = (char *) malloc(IPSIZE)) == NULL) return 1;
    if ((args->regUDP = (char *) malloc(TCPSIZE)) == NULL) return 1;

/* TODO    if ((info = (struct net_info *) malloc(sizeof(struct net_info))) == NULL)return 1;
    if ((info->ext_TCP = (char *) malloc(TCPSIZE)) == NULL)return 1;
    if ((info->ext_IP = (char *) malloc(IPSIZE)) == NULL)return 1;
    if ((info->rec_TCP = (char *) malloc(TCPSIZE)) == NULL)return 1;
    if ((info->rec_IP = (char *) malloc(IPSIZE)) == NULL)return 1; */

    if ((command = (char *) malloc(CMDSIZE)) == NULL) return 1;

    return 0;
}

/***********************************************************************************************************************
 * Function to free the dynamic memory required by the program
 * @param args struct containing information about the node
 * @param info struct containing information about the net connecting
 * @param command buffer used when reading commands from stdin
 **********************************************************************************************************************/
void info_free(struct my_info *args, struct net_info *info, char *command) {

    free(args->IP);
    free(args->regIP);
    free(args->TCP);
    free(args->regUDP);
    free(args);

    /* TODO free(info->rec_TCP);
    free(info->rec_IP);
    free(info->ext_IP);
    free(info->ext_TCP);*/
    free(command);

    return;

}
