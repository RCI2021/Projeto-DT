//
// Created by anton on 18/04/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_interface.h"
#include "net.h"


/***********************************************************************************************************************
 * Verify that all arguments passed in the command line are valid
 * @param args Struct containing information about node IP and TCP, as well as IP and UDP of the node Server
 * @param argc Number of arguments passed in the command line
 * @return 0 if OK, number of bad argument else
 **********************************************************************************************************************/
int arg_verify(struct my_info *args, int argc, char **argv) {

    unsigned int b1, b2, b3, b4, port;
    char aux[128];


    if (argc == 3) {

        strcpy(args->IP, argv[1]);
        strcpy(args->TCP, argv[2]);
        strcpy(args->regIP, UDPSERVER);
        strcpy(args->regUDP, UDPPORT);

    } else if (argc == 5) { //Is the number of arguments correct?

        strcpy(args->IP, argv[1]);
        strcpy(args->TCP, argv[2]);
        strcpy(args->regIP, argv[3]);
        strcpy(args->regUDP, argv[4]);

    } else {

        printf("\aNot enough arguments!\n Should be: ndn <ip_addr> <tcp_port> <reg_ip_addr> <reg_udp_port>\n");
        return -1;

    }    //Check argv[1] for Node´s IP Address
    if (sscanf(args->IP, "%u.%u.%u.%u%s", &b1, &b2, &b3, &b4, aux) != 4 || b1 > 255 || b2 > 255 || b3 > 255 ||
        b4 > 255) {
        printf("\aError: Bad <ip_addr>\n");   //LAMP
        return 1;
    }
    //Check argv[2] for Node´s TCP Port
    if (sscanf(args->TCP, "%u%s", &port, aux) != 1 || port > 65535) {
        printf("\aError: Bad <tcp_port>\n");  //LAMP
        return 2;
    }
    //Check argv[3] for IP Address of Node Server
    if (sscanf(args->regIP, "%u.%u.%u.%u%s", &b1, &b2, &b3, &b4, aux) != 4 || b1 > 255 || b2 > 255 || b3 > 255 ||
        b4 > 255) {
        printf("\aError: Bad <reg_ip_addr>\n"); //LAMP
        return 3;
    }
    //Check argv[4] for UDP Port of Node Server
    if (sscanf(args->regUDP, "%u%s", &port, aux) != 1 || port > 65535) {
        printf("\aError: Bad <udp_port>\n");  //LAMP
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
enum state_main command_handle(char *command, struct my_info *args, struct net_info *info) {

    char aux[6];
    short int n;

    strcpy(info->rec_IP, args->IP);
    strcpy(info->rec_TCP, args->TCP);

    if ((n = sscanf(command, "%s %d %d %s %s", aux, &info->net, &info->id, info->ext_IP, info->ext_TCP)) == 3) {

        strcpy(info->ext_IP, args->IP);
        strcpy(info->ext_TCP, args->TCP);

    }

    if ((strcmp(aux, "join") == 0) && ((n == 5) || (n == 3))) { //Case join

        if (n == 5) {
            return join;
        } else return get_nodeslist;

    } else if ((strcmp(aux, "exit") == 0) && (n == 1)) {   //Case exit
        return quit;
    } else {    //Other commands only work when connected to a net
        printf("Unknown Command, Available commands are:\n\t\t\t join \n\t\t\t exit\n");
        return wait;
    }
}

void ui_create(char *buffer, struct Cache *local, int id) {

    char buffer_name[BUFFERSIZE];

    sscanf(buffer, "%*s %s", buffer_name); //Separate the subname from the command
    sprintf(buffer, "%d.%s", id, buffer_name); //join the id with the subname before saving

    if (cache_search(buffer, local) < 0) { //Do we have a file with the same name already stored?

        cache_add(buffer, local); //Add the new name to the cache
        printf("Created %s\n", buffer); //Confirm the creation to the user

    } else printf("File already exists\n"); //If file already exists, do not overwrite

    return;
}

struct interest_list *
ui_get(char *buffer, struct Cache **local, struct Cache **cache, struct interest_list *interestList, exp_tree *tree,
       int my_id) {

    char buffer_name[BUFFERSIZE];
    int buffer_id, fd;

    if ((sscanf(buffer, "%*s %s", buffer_name) != 1) || sscanf(buffer_name, "%d", &buffer_id) != 1) {
        //Can´t read id & subname
        printf("Wrong file name format, format should be 'get id.subname'\n");

    } else {
        if (buffer_id == my_id) {
            if (cache_search(buffer_name, *local) >= 0) printf("DATA %s\n", buffer_name);
            else printf("NODATA %s", buffer_name);
        } else {
            if (cache_search(buffer_name, *cache) >= 0)printf("DATA %s\n", buffer_name);
            else {
                sprintf(buffer, "INTEREST %s\n", buffer_name);
                if ((fd = find_socket(buffer_id, tree)) == -1) printf("Node not found!\n");
                else TCP_send(buffer, fd);
                interestList = insertInterest(interestList, fd, buffer_name);
            }
        }
    }
    return interestList;
}

