//
// Created by anton on 23/03/2021.
//

#ifndef PROJETO_DT_UDP_H
#define PROJETO_DT_UDP_H

#define UDPSERVER "tejo.tecnico.ulisboa.pt"
#define UDPPORT 59000

int joinAlloc(char *buffer, char *message, char *net, char *ip, char *tcp, char *nodeip, char *nodetcp);

void joinFree(char *buffer, char *message, char *net, char *ip, char *tcp, char *nodeip, char *nodetcp);

int UDPcomms(char *message, char *buffer);

#endif //PROJETO_DT_UDP_H
