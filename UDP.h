//
// Created by anton on 23/03/2021.
//

#ifndef PROJETO_DT_UDP_H
#define PROJETO_DT_UDP_H

#define UDPSERVER "tejo.tecnico.ulisboa.pt"
#define UDPPORT "59000"

enum State {
    reg, list, error, get_peer, done
};

struct S_args {
    char *IP;
    char *TCP;
    char *regIP;
    char *regUDP;
};

struct net_info {
    char *ext_IP;
    char *ext_TCP;
    char *rec_IP;
    char *rec_TCP;
    struct socket_list list;
};

int joinAlloc(char *buffer, char *message, char *ip, char *tcp, char *nodeip, char *nodetcp);

void joinFree(char *buffer, char *message, char *ip, char *tcp, char *nodeip, char *nodetcp);

int UDPcomms(char *message, char *buffer);

#endif //PROJETO_DT_UDP_H
