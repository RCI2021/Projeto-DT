//
// Created by anton on 29/03/2021.
//

#include "command.h"
#include "client.h"


int join(char *command) {

    char *buffer, *message, *net, *nodeip, *nodetcp;
    extern char *id, *tcp;
    state = reg;

    if ((joinAlloc(buffer, message, net, id, tcp, nodeip, nodetcp)) != 0) state = error;

    switch state {
        case reg:
            sscanf(command, "%*s %s %s %s", net, id, tcp);
            ssprintf(message, "%s %s %s %s", REG, net, id, tcp);
            if (UDPcomms(message, buffer) != 0) state = error;
            if (!strcmp(buffer, OKREG)) state = error;
            else state = list;
            break;

        case list:
            ssprintf(message, "%s %s", NODESLIST, net)
            if (UDPcomms(message, buffer) != 0) state = error;
            if (sscanf(buffer, "NODESLIST %*d\n %s %s", nodeip, nodetcp) != 2) state = error;
            else state = advertise;
            break;

        case connect:
            break;

        case error:
            perror("Error during Nodes Server Operations");
            joinFree(buffer, message, net, id, tcp, nodeip, nodetcp);
            break;
    }

    joinFree(buffer, message, net, id, tcp, nodeip, nodetcp);
    return 0;
}

int joinAlloc(char *buffer, char *message, char *net, char *ip, char *tcp, char *nodeip, char *nodetcp) {

    if ((buffer = (char *) malloc(sizeof BUFFERSIZE + 1)) == NULL) perror("MEM ALLOCATION ERROR");
    return -1;
    if ((message = (char *) malloc(sizeof NODESLIST + sizeof net)) == NULL) perror("MEM ALLOCATION ERROR");
    return -1;
    if ((net = (char *) malloc(sizeof BUFFERSIZE + 1)) == NULL) perror("MEM ALLOCATION ERROR");
    return -1;
    if ((ip = (char *) malloc(sizeof BUFFERSIZE + 1)) == NULL) perror("MEM ALLOCATION ERROR");
    return -1;
    if ((tcp = (char *) malloc(sizeof BUFFERSIZE + 1)) == NULL) perror("MEM ALLOCATION ERROR");
    return -1;
    if ((nodeip = (char *) malloc(sizeof BUFFERSIZE + 1)) == NULL) perror("MEM ALLOCATION ERROR");
    return -1;
    if ((nodetcp = (char *) malloc(sizeof BUFFERSIZE + 1)) == NULL) perror("MEM ALLOCATION ERROR");
    return -1;

    return 0;
}

void joinFree(char *buffer, char *message, char *net, char *ip, char *tcp, char *nodeip, char *nodetcp) {

    free(buffer);
    free(message);
    free(net);
    free(ip);
    free(tcp);
    free(nodeip);
    free(nodetcp);

}


int leave() {

    char *message, *buffer;
    bool err;

    if ((message = (char *) malloc(sizeof UNREG)) == NULL) return -137;
    if ((buffer = (char *) malloc(BUFFERSIZE)) == NULL) return -137;

    ssprintf(message, UNREG);
    UDPcomms(message, buffer); //TODO have UDP & ServerIp as extern
    if (!strcmp(buffer, UNREG)) perror("UNREG not received");
    err = TRUE;

    free(message);
    free(buffer);
    return (int) err;

}

int create(char *command) {

    char *subname, *name;

    sscanf("%*s %s", name);
    ssprintf(name, "%d%s", id, subname);


}
