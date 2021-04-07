//
// Created by anton on 31/03/2021.
//

#ifndef PROJETO_DT_TCP_H
#define PROJETO_DT_TCP_H


int TCP_init(char *IP, char *port);
int TCP_server(char *IP, char *port);

/////
int FD_setlist(struct node *head_fd, fd_set *rfds);
#endif //PROJETO_DT_TCP_H
