
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_interface.h"
#include "registration.h"
#include "definition.h"
#include "net.h"
#include "linked_list.h"
#include "expedition.h"

//Function Definitions
int info_alloc(struct my_info *args, struct net_info *info); //Memory Allocation
void info_free(struct my_info *args, struct net_info *info); //Memory "Deallocation"

int main(int argc, char **argv) {

    struct my_info args;    //Info about myself
    struct net_info info;   //Info about the net I´m in
    char command[CMDSIZE];  //Command Buffer
    int ext_fd;
    enum state_main state = wait;   //State switch
    struct socket_list *list = NULL;
    exp_tree *tree = NULL;

    printf("%s %s %s %s", argv[1], argv[2], argv[3], argv[4]);

    if (info_alloc(&args, &info) != 0) return -1; //Memory allocation
    if (arg_verify(&args, argc, argv) != 0) return -1;   //Argument Verification

    while (state != quit) {
        switch (state) {
            case wait:
                printf("\nInput Command:");
                fgets(command, CMDSIZE, stdin);
                state = command_handle(command, &args, &info);
                break;

            case get_nodeslist:
                if (nodeslist(&args, &info) != 0) state = err;
                else state = join;
                break;

            case join:
                if ((strcmp(info.ext_IP, args.IP) != 0) || (strcmp(info.ext_TCP, args.TCP) != 0)) {

                    if ((ext_fd = TCP_client(&info, list, tree)) <= 0) {

                        state = err; //Not alone, may connect to other
                        break;
                    }

                }
                if (reg(&args, &info) != 0) {
                    state = err;
                    break;
                }
                state = connected;

                break;

            case connected:
                printf("CONNECTED\n");
                if (TCP_server(&args, &info, list, tree) != 0) {
                    state = err;
                    break;
                }
                if (unreg(&args, &info) != 0) {
                    state = err;
                    break;
                } else state = wait;
                break;
            case err:
                info_free(&args, &info);
                return -1;
                break;

            default:
                state = wait;
                break;
        }
    }

    info_free(&args, &info);
    return 0;

}


/***********************************************************************************************************************
 * Function to allocate memory required throughout the program
 * @param args struct containing information about the node
 * @param info struct containing information about the net connecting
 * @param command buffer used when reading commands from stdin
 * @return 0 if OK, 1 else
 **********************************************************************************************************************/
int info_alloc(struct my_info *args, struct net_info *info) {

    if ((args->IP = (char *) malloc(IPSIZE * sizeof(char))) == NULL) return 1;
    if ((args->TCP = (char *) malloc(TCPSIZE * sizeof(char))) == NULL) return 1;
    if ((args->regIP = (char *) malloc(IPSIZE * sizeof(char))) == NULL) return 1;
    if ((args->regUDP = (char *) malloc(TCPSIZE * sizeof(char))) == NULL) return 1;

    if ((info->ext_TCP = (char *) malloc(TCPSIZE)) == NULL)return 1;
    if ((info->ext_IP = (char *) malloc(IPSIZE)) == NULL)return 1;
    if ((info->rec_TCP = (char *) malloc(TCPSIZE)) == NULL)return 1;
    if ((info->rec_IP = (char *) malloc(IPSIZE)) == NULL)return 1;


    return 0;
}

/***********************************************************************************************************************
 * Function to free the dynamic memory required by the program
 * @param args struct containing information about the node
 * @param info struct containing information about the net connecting
 * @param command buffer used when reading commands from stdin
 **********************************************************************************************************************/
void info_free(struct my_info *args, struct net_info *info) {

    free(args->IP);
    free(args->regIP);
    free(args->TCP);
    free(args->regUDP);

    free(info->rec_TCP);
    free(info->rec_IP);
    free(info->ext_IP);
    free(info->ext_TCP);

    return;

}
