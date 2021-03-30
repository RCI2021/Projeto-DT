//
// Created by anton on 26/03/2021.
//

#ifndef PROJETO_DT_MSG_H
#define PROJETO_DT_MSG_H
#define DEBUG
#define BUFFERSIZE 1024
//UDP
#define NODESLIST "NODESLIST"
#define REG "REG"
#define UNREG "UNREG"
#define OKREG "OKREG"
#define OKUNREG "OKUNREG"
#define TIMEOUT 300
//TCP



//Database
struct database {
    char *name[DBSIZE];//todo
    int last_used;
    //FILE something something
};
#define MAXNAMESIZE 128
#define DBSIZE 2
#endif //PROJETO_DT_MSG_H
