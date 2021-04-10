//
// Created by António and Duarte on 23/03/2021.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "UDP.h"

enum state_main {

    wait, join_UDP, connected, quit

};

//Function Definitions
int info_alloc(struct my_info *args, struct net_info *info, char *command); //Memory Allocation
void info_free(struct my_info *args, struct net_info *info, char *command); //Memory "Deallocation"
enum state_main command_handle(char *command, struct net_info *info);    //Handle commands given by the user
int arg_verify(struct my_info *args, int argc, char **argv); //Argument Verification

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

/***********************************************************************************************************************
 * Function to understand what command was given by the user and if needed, other arguments that were passed
 * @param command original command read from stdin
 * @param info struct where info about the net might be stored
 * @return state to which the state should change
 **********************************************************************************************************************/
enum state_main command_handle(char *command, struct net_info *info) {

    char *aux;

    if ((aux = (char *) malloc(BUFFERSIZE)) == NULL) perror("Unknown Command");
    return wait;

    //Divide the command in its parts
    sscanf(command, "%s %d %d %s %s", aux, &info->net, &info->id, info->ext_IP, info->ext_TCP);

    //Compare command with available options
    if (strcmp(aux,
               "join")) { //Case join TODO separate join from join without UDP (Fill extern and switch to connected?
        return join_UDP;
    } else if (strcmp(aux, "exit")) {   //Case exit
        return quit;
    } else {    //Other commands only work when connected to 1 net
        printf("Unknown Command, Availabe commands are:\n\t\t\t join \n\t\t\t exit");
        return wait;
    }
}

/***********************************************************************************************************************
 * Verify that all arguments passed in the command line are valid
 * @param args Struct containing information about node IP and TCP, as well as IP and UDP of the node Server
 * @param argc Number of arguments passed in the command line
 * @return 0 if OK, number of bad argument else
 **********************************************************************************************************************/
int arg_verify(struct my_info *args, int argc, char **argv) {

    unsigned int b1, b2, b3, b4, port;
    char aux[128];

    if (argc != 5) { //Is the number of arguments correct?
        printf("Not enough arguments!\n Should be: ndn <ip_addr> <tcp_port> <reg_ip_addr> <reg_udp_port>\n");
        return -1;
    }

    strcpy(args->IP, argv[1]);
    strcpy(args->TCP, argv[2]);
    strcpy(args->regIP, argv[3]);
    strcpy(args->regUDP, argv[4]);
    //Check argv[1] for Node´s IP Address
    if (sscanf(args->IP, "%u.%u.%u.%u%s", &b1, &b2, &b3, &b4, aux) != 4 || b1 > 255 || b2 > 255 || b3 > 255 ||
        b4 > 255) {
        printf("Error: Bad <ip_addr>\n");   //LAMP
        return 1;
    }
    //Check argv[2] for Node´s TCP Port
    if (sscanf(args->TCP, "%u%s", &port, aux) != 1 || port > 65535) {
        printf("Error: Bad <tcp_port>\n");  //LAMP
        return 2;
    }
    //Check argv[3] for IP Address of Node Server
    if (sscanf(args->regIP, "%u.%u.%u.%u%s", &b1, &b2, &b3, &b4, aux) != 4 || b1 > 255 || b2 > 255 || b3 > 255 ||
        b4 > 255) {
        printf("Error: Bad <reg_ip_addr>\n"); //LAMP
        return 3;
    }
    //Check argv[4] for UDP Port of Node Server
    if (sscanf(args->regUDP, "%u%s", &port, aux) != 1 || port > 65535) {
        printf("Error: Bad <udp_port>\n");  //LAMP
        return 4;
    }
    return 0;
}