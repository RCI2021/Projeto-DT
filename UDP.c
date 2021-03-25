//
// Created by anton on 23/03/2021.
//

#include "UDP.h"

int join(char *net, int id){

    UDPget(net);
    UDPreg(net,id);

}

int joinDS(char *net, int id,char *bootIP, char *bootTCP){

    //TODO
    UDPreg(net,id);

}

int UDPget(net){



}