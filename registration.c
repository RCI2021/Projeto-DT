//
// Created by anton on 18/04/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include "registration.h"

/***********************************************************************************************************************
 * Function to Register a node in the UDP server, and get a neighbour to connect
 * @param args Struct containing info about the node being registered
 * @param info Struct containing info about the node´s neighbourhood
 * @return 0 if OK, non 0 else
 **********************************************************************************************************************/
int reg(struct my_info *args, struct net_info *info) {

    char *message, *buffer;

    if ((message = (char *) malloc(BUFFERSIZE * sizeof(char))) == NULL) return -1; //Memory Allocation
    if ((buffer = (char *) malloc(BUFFERSIZE * sizeof(char))) == NULL) return -1;

    sprintf(message, "REG %d %s %s", info->net, args->IP, args->TCP); //Construct REG message
    if ((UDP_exch(message, buffer, args)) != 0) { //Send Reg message
        perror("UDP Error");    //Check for errors within sending the message
        free(message);
        free(buffer);
        return -1;
    }
    if (strcmp(buffer, "OKREG") != 0) { //Check if OKREG was received
        perror("timeout");
        free(message);
        free(buffer);
        return -1;
    }
    fputs(buffer, stdin);
    free(message);
    free(buffer);
    return 0;
}

int nodeslist(struct my_info *args, struct net_info *info) {


    char *message, *buffer;

    if ((message = (char *) malloc(BUFFERSIZE * sizeof(char))) == NULL) return -1; //Memory Allocation
    if ((buffer = (char *) malloc(BUFFERSIZE * sizeof(char))) == NULL) return -1;

    if ((strcmp(info->ext_IP, "X") == 0) &&
        (strcmp(info->ext_TCP, "X") == 0)) { //Getting Extern, if one wasn´t provided by the yser
        sprintf(message, "NODES %d", info->net);    //Get nodes list from Server
        if ((UDP_exch(message, buffer, args)) != 0) {
            free(message); //if Error
            free(buffer);
            return -1;
        }
        sscanf(buffer, "%*s %*d %s %s", info->ext_IP, info->ext_TCP);   //Get neighbour from buffer
    }

    free(message);
    free(buffer);
    return 0;

}

int unreg(struct my_info *args, struct net_info *info) {

    char *message, *buffer;

    if ((message = (char *) malloc(BUFFERSIZE)) == NULL) return -1;
    if ((buffer = (char *) malloc(BUFFERSIZE)) == NULL) return -1;

    sprintf(message, "UNREG %d %s %s", info->net, args->IP, args->TCP);
    UDP_exch(message, buffer, args);
    if (strcmp(buffer, "OKUNREG") != 0) {
        perror("OKUNREG not received");
        free(message);
        free(buffer);
        return -1;
    }

    free(message);
    free(buffer);
    return 0;

}

/***********************************************************************************************************************
 * Function to exchange messages between client and UDP server
 * @param message message the program whishes to transmit to the server
 * @param buffer response given by the server
 * @param args Struct containing info about the server
 * @return 0 if ok, non 0 else
 **********************************************************************************************************************/
int UDP_exch(char *message, char *buffer, struct my_info *args) {

    struct addrinfo hints, *res;
    int serverfd;
    struct sockaddr addr;
    socklen_t addrlen;
    ssize_t n;
    fd_set rfds;
    int retval;

    if ((serverfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {   //Create Socket
        perror("Error creating socket");
        return -1;
    }

    memset(&hints, 0, sizeof hints);  //Set hints
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if ((getaddrinfo(args->regIP, args->regUDP, &hints, &res)) != 0) {    //Get server address info
        perror("Error addrinfo UDP");
        close(serverfd);
        freeaddrinfo(res);
        return -1;
    }

    if ((n = sendto(serverfd, message, strlen(message), 0, res->ai_addr, res->ai_addrlen)) == -1) { //Send message
        perror("Error sending message to UDP server");
        close(serverfd);
        freeaddrinfo(res);
        return -1;
    }

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    FD_SET(serverfd, &rfds);
    //Select to multiplex between user given quit and response received from server
    retval = select(serverfd + 1, &rfds, (fd_set *) NULL, (fd_set *) NULL, (struct timeval *) NULL);

    if (retval < 1) {   //Error in select
        perror("Select Error");
        return -1;
    } else if (FD_ISSET(serverfd, &rfds)) { //Server response comes first

        addrlen = sizeof addr;
        if ((n = recvfrom(serverfd, buffer, BUFFERSIZE, 0, &addr, &addrlen)) == -1) {
            perror("Error Retrieving message from UDP server");
            close(serverfd);
            freeaddrinfo(res);
            return -1;
        } else buffer[n] = '\0';

    } else if (FD_ISSET(0, &rfds)) { //User interrupts process, or calls another command

        fgets(buffer, BUFFERSIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    }

    close(serverfd);
    freeaddrinfo(res);
    return 0;

}
