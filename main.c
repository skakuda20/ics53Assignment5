#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "Client Domain/Client.c"
#include "Server Domain/Server.c"
int main()
{
    printf("Start...\n");
    pid_t server_pid = fork ();
    if (server_pid == 0){
        start_server();
        exit(0);
    }
    pid_t client_pid = fork ();
    if (client_pid == 0){
        start_client();
        exit(0);
    }
    waitpid (server_pid, NULL, 0);
    waitpid (client_pid, NULL, 0);
    printf("Exiting...\n");
    return 0;
}