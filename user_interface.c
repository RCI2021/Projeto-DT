//
// Created by anton on 18/04/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_interface.h"

/***********************************************************************************************************************
 * Verify that all arguments passed in the command line are valid
 * @param args Struct containing information about node IP and TCP, as well as IP and UDP of the node Server
 * @param argc Number of arguments passed in the command line
 * @return 0 if OK, number of bad argument else
 **********************************************************************************************************************/
int arg_verify(struct my_info *args, int argc, char **argv) {

    unsigned int b1, b2, b3, b4, port;
    char aux[128];

    strcpy(args->IP, argv[1]);
    strcpy(args->TCP, argv[2]);

    if (argc == 3) {

        strcpy(args->regIP, UDPSERVER);
        strcpy(args->regUDP, UDPPORT);

    } else if (argc == 5) { //Is the number of arguments correct?

        strcpy(args->regIP, argv[3]);
        strcpy(args->regUDP, argv[4]);

    } else {

        printf("Not enough arguments!\n Should be: ndn <ip_addr> <tcp_port> <reg_ip_addr> <reg_udp_port>\n");
        return -1;

    }    //Check argv[1] for Node´s IP Address
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


/***********************************************************************************************************************
 * Function to understand what command was given by the user and if needed, other arguments that were passed
 * @param command original command read from stdin
 * @param info struct where info about the net might be stored
 * @return state to which the state should change
 **********************************************************************************************************************/
enum state_main command_handle(char *command, struct net_info *info) {

    char aux[6];
    short int n;

    if ((n = sscanf(command, "%s %d %d %s %s", aux, &info->net, &info->id, info->ext_IP, info->ext_TCP)) == 3) {
        strcpy(info->ext_IP, "X");
        strcpy(info->ext_TCP, "X");
        return get_nodeslist;

    } else if ((n != 5) && (n != 1)) {

        printf("Wrong number of arguments");
        return wait;

    } else if (strcmp(aux, "join") == 0) { //Case join TODO command leave
        return join;
    } else if (strcmp(aux, "exit") == 0) {   //Case exit
        return quit;
    } else {    //Other commands only work when connected to 0 net
        printf("Unknown Command, Available commands are:\n\t\t\t join \n\t\t\t exit");
        return wait;
    }


    //Divide the command in its parts

}

