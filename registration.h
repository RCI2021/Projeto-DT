//
// Created by anton on 18/04/2021.
//

#ifndef PROJETO_DT_REGISTRATION_H
#define PROJETO_DT_REGISTRATION_H

#define UDPSERVER "tejo.tecnico.ulisboa.pt"
#define UDPPORT "59000"
#define IPSIZE 100
#define TCPSIZE 100
#define BUFFERSIZE 128

struct my_info {
    char *IP;
    char *TCP;
    char *regIP;
    char *regUDP;
};

struct net_info {
    int net;
    int id;
    char *ext_IP;
    char *ext_TCP;
    char *rec_IP;
    char *rec_TCP;
    struct socket_list *list;
};

int reg(struct my_info *args, struct net_info *info); //Register Node on UDP server
int UDP_exch(char *message, char *buffer, struct my_info *args); //Send UDP message and retrieve response

#endif //PROJETO_DT_REGISTRATION_H
