//
// Created by anton on 26/03/2021.
//

#ifndef PROJETO_DT_MSG_H
#define PROJETO_DT_MSG_H

#define BUFFERSIZE 1024
//UDP
#define NODESLIST "NODESLIST"
#define REG "REG"
#define UNREG "UNREG"
#define OKREG "OKREG"
#define OKUNREG "OKUNREG"
#define TIMEOUT 300
//TCP
#define QUEUELENGHT 5
#define IPSIZE 1//TODO
#define TCPSIZE 2//TODO

//Database
#define DBSIZE 2

struct database {
    char *name[DBSIZE];//todo
    bool last_used;
    //FILE something something
};

#define MAXNAMESIZE 128
#endif //PROJETO_DT_MSG_H
