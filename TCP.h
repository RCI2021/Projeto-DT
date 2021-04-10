//
// Created by anton on 31/03/2021.
//

#ifndef PROJETO_DT_TCP_H
#define PROJETO_DT_TCP_H


int TCP_init(char *IP, char *port);
int TCP_server(char *IP, char *port);

/////
enum Keyboard_state net_keyboard_handle(char *buffer);

int FD_setlist(struct node *head_fd, fd_set *rfds);

enum net_state net_buffer_handle(char *buffer);
#endif //PROJETO_DT_TCP_H
