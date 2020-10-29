//
// Created by Mario Eberth on 29.10.20.
//
#include "UDPClient.h"

#include <iostream>
#include <sys/socket.h> // socket, bind, listen, accept
#include <netinet/in.h> // IPPROTO_TCP, sockaddr_in,
// htons/ntohs, INADDR_ANY
#include <unistd.h> // close
#include <arpa/inet.h> // inet_ntop/inet_atop
#include <string.h> // strlen
#include <semaphore.h> // sem_init

#define BUFFER_SIZE 1024

UDPClient::UDPClient(int _port, int _ipAddr) {
    ipPort = _port;
    ipAddr = _ipAddr;
}

void UDPClient::initializeSocket() {
    //Socket
    int _addressFormat = AF_INET;                                                   //Format Ipv4
    int _socketType = SOCK_DGRAM;                                                  //UDP
    int _socketProtocol = 0;                                                        //communication protocol > self check

    clientSocket =  socket(_addressFormat, _socketType, _socketProtocol);
}

void UDPClient::startSocket() {
    std::cout << "Ready to write" << std::endl;
    //recvfrom
    sockaddr_in from;
    socklen_t frommSize = sizeof(from);

    //send
    sockaddr_in toAddr;
    toAddr.sin_family = AF_INET;
    toAddr.sin_port = ipPort;
    toAddr.sin_addr.s_addr = ipAddr;
    memset(&(toAddr.sin_zero),'\0',0);
    int toSize = sizeof(toAddr);

    char msg[BUFFER_SIZE];
    memset(msg, '\0' , sizeof(msg));
    while (strcmp(msg, "exit") != 0 && strcmp(msg, "shutdown") != 0){
        //send
        std::cout << "Bitte um Eingabe: ";
        std::cin.getline(msg,BUFFER_SIZE);                                     //get the input line and save it into msg
        strcat(msg, "\0");
        int msgSize = strlen(msg) + 1;

        if (sendto(clientSocket, msg, msgSize,0,(sockaddr*) &toAddr, toSize) == -1){
            std::cout << "Fehler in der Send" << std::endl;
        }
        //recvfrom
        char receiveMsg[BUFFER_SIZE];
        if (recvfrom(clientSocket, receiveMsg, BUFFER_SIZE, 0, (sockaddr*) &from, &frommSize) >=0 ){
            std::cout << receiveMsg << std::endl;
        }
    }
    if (close(clientSocket) == -1){
        std::cout << "Fehler in der Close" << std::endl;                    //closesocket
    }
}