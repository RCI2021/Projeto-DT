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
#define CREATE "create"
#define GET "get"
#define ST1 "show topology"
#define SR1 "show routing"
#define SC1 "show cache"
#define ST2 "st"
#define SR2 "sr"
#define SC2 "sc"
#define LV "leave"
#define EXT "EXTERN"
#define ADV "ADVERTISE"
#define WIT "WITHDRAW"
#define INTEREST "INTEREST"
#define D "DATA"
#define NOD "NODATA"

struct database {
    char *name[DBSIZE];//todo
    bool last_used;
    //FILE something something
};

#define MAXNAMESIZE 128
#endif //PROJETO_DT_MSG_H