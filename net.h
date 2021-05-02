//
// Created by anton on 20/04/2021.
//

#ifndef PROJETO_DT_NET_H
#define PROJETO_DT_NET_H

#include "definition.h"
#include "expedition.h"

int TCP_client(struct net_info *info, struct socket_list *list, exp_tree **tree, struct my_info *my_info);

void show_topology(struct net_info *info);

int send_adv(int id, int fd);

int TCP_server(struct my_info *args, struct net_info *info, int ext_fd, struct socket_list *list, exp_tree **tree);

int TCP_rcv(int fd, char *buffer);

void TCP_send_all(char *buffer, struct socket_list *list, int fd);

void TCP_send(char *buffer, int fd);

int extern_update(struct net_info *info, struct my_info *args, char *buffer,char *client_IP,char *client_TCP);



#endif //PROJETO_DT_NET_H
