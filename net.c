//
// Created by António on 20/04/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include "net.h"
#include "registration.h"

int TCP_client(struct net_info *info) { //RETURN FD

    fd_set rfds;
    struct addrinfo hints, *res;
    int fd, errcode;
    ssize_t nbytes, nleft, nwritten, nread;
    struct timeval tv;
    char *ptr, buffer[BUFFERSIZE]; //Question ptr não devia ser inteiro

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) return -1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    errcode = getaddrinfo(info->ext_IP, info->ext_TCP, &hints, &res);
    if (errcode != 0) return -1;

    errcode = connect(fd, res->ai_addr, res->ai_addrlen);
    if (errcode == -1) return -1;

    sprintf(buffer, "NEW %d\n", info->id);
    ptr = &buffer[0];  //question este ptr é para q?
    nbytes = (4 + sizeof(info->id));

    nleft = nbytes;
    while (nleft > 0) {
        nwritten = write(fd, ptr, nleft);
        if (nwritten <= 0) return -1;
        nleft -= nwritten;
        ptr += nwritten;
    }
    nbytes = 6; //question EXT +3?
    nleft = nbytes;
    ptr = buffer;

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    FD_SET(fd, &rfds);

    tv.tv_usec = 60;
    errcode = select(fd + 1, &rfds, (fd_set *) NULL, (fd_set *) NULL, &tv);

    if (errcode <= 0) {
        perror("Select error");
        freeaddrinfo(res);
        return -1;
    }

    if (FD_ISSET(fd, &rfds)) {
        while (nleft > 0) {
            nread = read(fd, ptr, nleft);
            if (nread == -1) return -1;
            else if (nread == 0) break;
            nleft -= nread;
            ptr += nread;
        }
        nread = nbytes - nleft;
        buffer[nread] = '\0';
    }
    if (FD_ISSET(0, &rfds)) {
        fgets(buffer, BUFFERSIZE, stdin);
        if (strcmp(buffer, "cancel") != 0) {
            printf("New Operation Canceled");
            return -1;
        }
    }

    if (strncmp(buffer, "EXT", 3) != 0) {

        printf("Expected Extern, received %s", buffer);
        freeaddrinfo(res);
        return -1;

    } else sscanf(buffer, "%*s %s %s", info->rec_IP, info->rec_TCP);

    freeaddrinfo(res);
    return fd; //write NEW IP TCP read IP TCP -> recovery
}

