#include <iostream>
#include <sys/socket.h> // socket, bind, listen, accept
#include <netinet/in.h> // IPPROTO_TCP, sockaddr_in,
// htons/ntohs, INADDR_ANY
#include <unistd.h> // close
#include <arpa/inet.h> // inet_ntop/inet_atop
#include <string.h> // strlen
#include <semaphore.h> // sem_init

#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H
#define BUFFER_SIZE 1024

#define IPPORT (_argv[1])
#define IPADDRESS (_argv[2])

#include "TCPClient.h"
#include "UDPClient.h"
#include "TCPv6Client.h"

void printCommandLine(){
    std::cout << "C++ Client Menu:" << std::endl;
    std::cout << "----------------------" << std::endl;
    std::cout << "1. Start TCP v4 Client" << std::endl;
    std::cout << "2. Start TVP v6 Client" << std::endl;
    std::cout << "3. Start UDP Echo Client" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "----------------------" << std::endl;
}

int main(int _argc, char **_argv) {
    int port = htons(atoi(IPPORT));

    printCommandLine();

    while (true){
        std::cout << "Choose server type to start: ";

        char msg[BUFFER_SIZE];
        std::cin.getline(msg,BUFFER_SIZE);                                     //get the input line and save it into msg
        strcat(msg, "\0");

        if (strcmp(msg, "1") == 0){
            TCPClient *s1 = new TCPClient(port, inet_addr(IPADDRESS));
            s1->initializeSocket();
            s1->startSocket();
            delete s1;
        } else if(strcmp(msg, "2")== 0){
            TCPv6Client *v6 = new TCPv6Client(port,inet_addr(IPADDRESS));
            v6->initializeSocket();
            v6->startSocket();
            delete v6;
        } else if(strcmp(msg, "3")== 0){
            UDPClient *u1 = new UDPClient(port, inet_addr(IPADDRESS));
            u1->initializeSocket();
            u1->startSocket();
            delete u1;
        } else if(strcmp(msg, "4")== 0){
            exit(0);
        }
    }
}

#endif
