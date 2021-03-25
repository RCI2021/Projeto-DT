//
// Created by António and Duarte on 23/03/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

#define IPSIZE 24 //TODO

struct S_args{

    char *IP;
    char *TCP;
    char *regIP;
    char *regUDP;

};

int main(int argc, char **argv){

    struct S_args *args;
    char *command, *name, *object, *net, bootIP, bootTCP;
    int *id;
    short int c;

    if((args=(struct S_args *) malloc(sizeof(struct S_args)))==NULL) exit(137); // Allocating mem for Arguments Struct
    if((command=(char *) malloc(sizeof(char)*7))==NULL) exit(137);  // Allocating mem for command string
    if((name=(char *) malloc(sizeof(char)*7))==NULL) exit(137);  // Allocating mem for name string
    if((id=(int *) malloc(sizeof(int)))==NULL) exit(137);  // Allocating mem for id integer

    strcpy(args->IP,argv[1]);
    strcpy(args->TCP,argv[2]);
    strcpy(args->regIP,argv[3]);
    strcpy(args->regUDP,argv[4]);

    while (!strcmp(command,"exit")){

        if(1==scanf("%s",command)){         /*TODO - AQUI NÃO DEVIA SER >1? OU OS OUTROS ARGUMENTOS SÃO DADOS A SEGUIR*/

            switch (command[0]) {

                case 'j':          //Join
                    net=malloc(sizeof()/*TODO*/);
                    bootIP=(char *) malloc(sizeof(IPSIZE));
                    bootTCP=(char *) malloc(sizeof(IPSIZE));
                    c=scanf("%s %d %s %s",net,id,bootIP,bootTCP);
                    if (c==2) join(net,id); //Possible use of variable argument number function?
                    else if (c==4) joinNS(net,id,bootIP,bootTCP);
                    else printf("Undefined Arguments, plz input net, id, bootIP and bootTCP");
                    break;

                case 'c':           //Create an object that contains the id of node and its subname
                    //itoa(id, object, 10);
                    strcat(object, '.');


                    break;
                case 'g':       //Starts search for object with the argument name
                    break;
                case 's':       //Show topology, routing or cache
                    switch (command[1]) {
                        case 't':
                            topology();
                            break;
                        case 'r':
                            routing();
                            break;
                        case 'c':
                            cache();
                            break;
                        default:
                            printf("Tell me what to show!");
                    }

                    break;
                case 'l':       //Leave node
                    break;
                default:
                    printf("Unknown Command");
                    break;
            }
        } else printf("PLZ INPUT SOMETHING");




    }

    free(args);
    free(command);
    exit(0);

}
