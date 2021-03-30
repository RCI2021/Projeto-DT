//
// Created by António and Duarte on 23/03/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include "client.h"
#include "UDP.h"

struct S_args {
    char *IP;
    char *TCP;
    char *regIP;
    char *regUDP;
};

int main(int argc, char **argv) {

    struct S_args *args;
    char *command, *name, *object, *net, *bootIP, *bootTCP;
    int *id;
    short int c;

    if ((args = (struct S_args *) malloc(sizeof(struct S_args))) == NULL)exit(137); // Allocating mem for Arguments
    if ((args->IP = (char *) malloc(sizeof(argv[1]))) == NULL) exit(137);
    if ((args->TCP = (char *) malloc(sizeof(argv[2]))) == NULL) exit(137);
    if ((args->regIP = (char *) malloc(sizeof(argv[3]))) == NULL) exit(137);
    if ((args->regUDP = (char *) malloc(sizeof(argv[4]))) == NULL) exit(137);
    if ((command = (char *) malloc(BUFFERSIZE)) == NULL) exit(137);

    strcpy(args->IP, argv[1]);
    strcpy(args->TCP, argv[2]);
    strcpy(args->regIP, argv[3]);
    strcpy(args->regUDP, argv[4]);
    argsVerification(args, argc);

    while (!strcmp(command, "exit")) {

        switch (CommandChoice(command)):

        case 1: //Join command
            join(command);
        break;
        case 2: //create
            create(command);
        break;
        case 3: //get
            break;
        case 4: //Show topology
            break;
        case 5: //Show routing
            break;
        case 6: //show cache
            break;
        case 7: //leave
            break;
        case 8: //exit
            break;
        case -1:    //unknown command
            break;
        default:    //undefined behaviour?
            break;


    }


    free(args);
    free(command);
    exit(0);

}

int argVerification(struct S_args *args, int argc) {

    unsigned int b1, b2, b3, b4, port;
    char aux[128];

    if (argc != 5) {
        printf("Not enough arguments!\n Should be: ndn <ip_addr> <tcp_port> <reg_ip_addr> <reg_udp_port>\n");
        exit(1);
    }
    //Checks argv[1] for IP Address
    if (sscanf(args->IP, "%u.%u.%u.%u%s", &b1, &b2, &b3, &b4, aux) != 4 || b1 > 255 || b2 > 255 || b3 > 255 ||
        b4 > 255) {
        printf("Error: Bad <ip_addr>\n");
        exit(1);
    }
    //Checks argv[2] for TCP Port
    if (sscanf(args->TCP, "%u%s", &port, aux) != 1 || port > 65535)     //TODO PORT IS args->TCP??
    {
        printf("Error: Bad <tcp_port>\n");
        exit(1);
    }
    //Checks argv[3] for IP Address of Node Server
    if (sscanf(args->regIP, "%u.%u.%u.%u%s", &b1, &b2, &b3, &b4, aux) != 4 || b1 > 255 || b2 > 255 || b3 > 255 ||
        b4 > 255)  //TODO b values only used for verification or saved for later?
    {
        printf("Error: Bad <reg_ip_addr>\n");
        exit(1);
    }
    //Checks argv[4] for UDP Port of Node Server
    if (sscanf(args->regUDP, "%u%s", &port, aux) != 1 || port > 65535)     //TODO PORT IS args->regUDP??
    {
        printf("Error: Bad <udp_port>\n");
        exit(1);
    }
}

int commandChoice(char *command) {

    if (fgets(command, BUFFERSIZE - 1, stdin) == 1) {

        if (strcmp(command, "join")) {
            return 1;
        } else if (strcmp(command, "create")) {
            return 2;
        } else if (strcmp(command, "get")) {
            return 3;
        } else if ((strcmp(command, "show topology")) || (strcmp(command, "st"))) {
            return 4;
        } else if ((strcmp(command, "show routing")) || (strcmp(command, "sr"))) {
            return 5;
        } else if ((strcmp(command, "show cache")) || (strcmp(command, "sc"))) {
            return 6;
        } else if (strcmp(command, "leave")) {
            return 7;
        } else if ((strcmp(command, "exit")) {
            return 8;
        } else return -1;
    }

}