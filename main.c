//
// Created by António and Duarte on 23/03/2021.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "client.h" //Definitions
#include "registration.h" //Join & leave
#include "user_interface.h" //User commands
#include "net.h" // Connection to the network

//Function Definitions
int info_alloc(struct my_info *args, struct net_info *info, char *command); //Memory Allocation
void info_free(struct my_info *args, struct net_info *info, char *command); //Memory "Deallocation"

int main(int argc, char **argv) {

    struct my_info *args = NULL;
    struct net_info *info = NULL;
    char *command = NULL;
    enum state_main state = wait;

    //Argument Handles
    info_alloc(args, info, command);
    arg_verify(args, argc, argv);

    while (state != quit) {
        switch (state) {

            case wait:
                fgets(command, BUFFERSIZE, stdin);
                state = command_handle(command, info);
                break;
            case join_UDP:
                if ((join(args, info)) != 0) {
                    perror("Error Joining net");
                    state = wait;
                }
                state = connected:
                break;
            case connected:
                if ((TCP_server(info)) != 0) {
                    perror("Net error");
                }
                state = wait;

                break;
            case quit:
                break;

        }
    }
    return 0;

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
    if ((info = (struct net_info *) malloc(sizeof(struct net_info))) == NULL)return 1;
    if ((info->ext_TCP = (char *) malloc(TCPSIZE)) == NULL)return 1;
    if ((info->ext_IP = (char *) malloc(IPSIZE)) == NULL)return 1;
    if ((info->rec_TCP = (char *) malloc(TCPSIZE)) == NULL)return 1;
    if ((info->rec_IP = (char *) malloc(IPSIZE)) == NULL)return 1;
    if ((command = (char *) malloc(BUFFERSIZE)) == NULL) return 1;

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
    free(info->rec_TCP);
    free(info->rec_IP);
    free(info->ext_IP);
    free(info->ext_TCP);
    free(command);

    return;

}
