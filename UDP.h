//
// Created by anton on 23/03/2021.
//

#ifndef PROJETO_DT_UDP_H
#define PROJETO_DT_UDP_H

#define UDPSERVER "tejo.tecnico.ulisboa.pt"
#define UDPPORT "59000"


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


int join(struct my_info *args, struct net_info *info);

int unreg(struct my_info *args, struct net_info *info);

int UDP_exch(char *message, char *buffer, struct my_info *args);

#endif //PROJETO_DT_UDP_H
