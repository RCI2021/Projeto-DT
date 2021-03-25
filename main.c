//
// Created by anton on 23/03/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>


struct S_args{

    char IP;
    char *TCP;
    char *regIP;
    char *regUDP;

};

int main(int argc, char **argv){

    struct S_args args;

    if(args=(* struct S_args) malloc(sizeof(struct S_args))==NULL) exit(137);


    strcpy(argv[1],args.IP);
    strcpy(argv[2],args.TCP);
    strcpy(argv[3],args.regIP);
    strcpy(argv[4],args.regUDP);

    while (!strcmp(comand,"exit")){

        scanf("%s",command);

        switch (command) {

            case join:
                break;
            case create:
                break;
            case get:
                break;
            case show,st,sr,sc:
                break;
            case leave:
                break;
            default:
                printf("Unknown Command");
                break;
        }
    }

    exit(0);
}