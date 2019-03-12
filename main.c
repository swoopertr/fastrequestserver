#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#define PORT 8080

const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain;\nContent-Length: 2;\nthedede:313131;\n\nOK";

void* returnInThread(void* prms){
    int new_socket = *((int*)prms);
    char buffer[30000] = {0};
    read( new_socket , buffer, 30000);

    write(new_socket , hello , strlen(hello));
    close(new_socket);
    return 0;
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        pthread_t thread_handle;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        int ret = pthread_create(&thread_handle, 0, returnInThread, (void*)&new_socket);
        if (ret != 0){
            printf("socket patladı");
        }
        pthread_join(thread_handle, 0);
    }
    return 0;
}