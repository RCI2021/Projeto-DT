//
// Created by anton on 20/04/2021.
//

#ifndef PROJETO_DT_DEFINITION_H
#define PROJETO_DT_DEFINITION_H

#define UDPSERVER "193.136.138.142"
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

#endif //PROJETO_DT_DEFINITION_H
