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
    char *command, *name, *object, *net, *bootIP, *bootTCP;
    int *id;
    short int c;

    //VARIABLES USED FOR VERIFICATION
    unsigned int b1, b2, b3, b4, port;
    char aux[128];
    //

    if((args=(struct S_args *) malloc(sizeof(struct S_args)))==NULL) exit(137); // Allocating mem for Arguments Struct
    if((command=(char *) malloc(sizeof(char)*7))==NULL) exit(137);  // Allocating mem for command string
    if((name=(char *) malloc(sizeof(char)*7))==NULL) exit(137);  // Allocating mem for name string
    if((id=(int *) malloc(sizeof(int)))==NULL) exit(137);  // Allocating mem for id integer

   if(argc != 5)
   {
       printf("Not enough arguments!\n Should be: ndn <ip_addr> <tcp_port> <reg_ip_addr> <reg_udp_port>\n");
       exit(1);
   }
   //Checks argv[1] for IP Address
   if(sscanf(argv[1], "%u.%u.%u.%u%s", &b1, &b2, &b3, &b4, aux) != 4 || b1>255 || b2>255 || b3>255 || b4>255)
   {
       printf("Error: Bad <ip_addr>\n");
       exit(1);
   }
   //Checks argv[2] for TCP Port
   if(sscanf(argv[2], "%u%s", &port, aux) != 1 || port > 65535)     //TODO PORT IS args->TCP??
   {
       printf("Error: Bad <tcp_port>\n");
       exit(1);
   }
   //Checks argv[3] for IP Address of Node Server
    if(sscanf(argv[3], "%u.%u.%u.%u%s", &b1, &b2, &b3, &b4, aux) != 4 || b1>255 || b2>255 || b3>255 || b4>255)  //TODO b values only used for verification or saved for later?
    {
        printf("Error: Bad <reg_ip_addr>\n");
        exit(1);
    }
    //Checks argv[4] for UDP Port of Node Server
    if(sscanf(argv[2], "%u%s", &port, aux) != 1 || port > 65535)     //TODO PORT IS args->regUDP??
    {
        printf("Error: Bad <tcp_port>\n");
        exit(1);
    }


    strcpy(args->IP,argv[1]);
    strcpy(args->TCP,argv[2]);
    strcpy(args->regIP,argv[3]);
    strcpy(args->regUDP,argv[4]);

    while (!strcmp(command,"exit")){

        switch (CommandChoice(command)):

        case 1: //Join command
            net = (int *) malloc(sizeof(int));
            bootIP=(char *) malloc(sizeof(IPSIZE));
            bootTCP=(char *) malloc(sizeof(IPSIZE));
            c=scanf("%s %d %s %s",net,id,bootIP,bootTCP);
            if (c==2) join(net,id); //Possible use of variable argument number function?
            else if (c==4) joinDS(net,id,bootIP,bootTCP);
            else printf("Undefined Arguments, plz input net, id, bootIP and bootTCP");
            break;
        case 2: //create
            //Create an object that contains the id of node and its subname
            //itoa(id, object, 10);
            strcat(object, '.');
            break;
        case 3: //get
            break;
        case 4: //Show topology
            break;
        case 5: //Show routing
            break;
        case 6: //show cache
            break;
        case 7: //leave
            break;
        case 8: //exit
            break;
        case -1:    //unknown command
            break;
        default:    //undefined behaviour?
            break;



    }


    free(args);
    free(command);
    exit(0);

}

int commandChoice(char *command){

    if(scanf("%s ",command)==1){

       if (strcmp(command,"join")){
           return 1;
       }else if (strcmp(command,"create")){
           return 2;
       }else if (strcmp(command,"get")){
           return 3;
       }else if ((strcmp(command,"show topology"))||(strcmp(command,"st"))){
           return 4;
       }else if ((strcmp(command,"show routing"))||(strcmp(command,"sr"))){
           return 5;
       }else if ((strcmp(command,"show cache"))||(strcmp(command,"sc"))) {
           return 6;
       }else if (strcmp(command,"leave")){
           return 7;
       }else if ((strcmp(command,"exit")){
           return 8;
       }else return -1;
    }

}