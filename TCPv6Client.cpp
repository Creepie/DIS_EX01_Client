//
// Created by Mario Eberth on 29.10.20.
//

#include "TCPv6Client.h"

#include <iostream>
#include <sys/socket.h> // socket, bind, listen, accept
#include <netinet/in.h> // IPPROTO_TCP, sockaddr_in,
// htons/ntohs, INADDR_ANY
#include <unistd.h> // close
#include <arpa/inet.h> // inet_ntop/inet_atop
#include <string.h> // strlen
#include <semaphore.h> // sem_init

#define BUFFER_SIZE 1024

TCPv6Client::TCPv6Client(int _port, int _ipAddr) {
    ipPort = _port;
    ipAddr = _ipAddr;
}

void TCPv6Client::initializeSocket() {
//Socket
    int _addressFormat = AF_INET6;                                                   //Format Ipv6
    int _socketType = SOCK_STREAM;                                                  //TCP
    int _socketProtocol = 0;                                                        //communication protocol > self check

    clientSocket = socket(_addressFormat, _socketType, _socketProtocol);        //create the client socket
}

void TCPv6Client::startSocket() {
    //connect
    struct sockaddr_in6 serverAddr;
    serverAddr.sin6_family = AF_INET6;
    serverAddr.sin6_flowinfo = 0;
    serverAddr.sin6_port = ipPort;
    serverAddr.sin6_scope_id = 0;

    if (inet_pton(AF_INET6, "::1", &(serverAddr.sin6_addr)) <1){
        std::cout << "Fehler bei convert" << std::endl;
    }

    if (connect(clientSocket, (sockaddr *) &serverAddr, sizeof(serverAddr)) >=0) {   //check is connection successful > errorCode >= 0
        char msg[BUFFER_SIZE];                                                      //creates a charArray
        memset(msg, '\0' , sizeof(msg));
        while (strcmp(msg, "exit") != 0 && strcmp(msg, "shutdown") != 0) {
            //read message
            std::cout << "Bitte um Eingabe: ";
            std::cin.getline(msg,BUFFER_SIZE);                                     //get the input line and save it into msg
            strcat(msg, "\0");
            int msgSize = strlen(msg) + 1;                                          //get the length of the msg
            //send
            if (!send(clientSocket, msg, msgSize, 0) >0) {                          //send the message and check the return value of the send method
                std::cout << "Error Sending message" << std::endl;
            }

            //receive
            char receiveMsg[BUFFER_SIZE];                                               //create a char array
            if (recv(clientSocket, receiveMsg, BUFFER_SIZE, 0) >0) {                    //recv the message and check the error code (return value)
                std::cout << receiveMsg << std::endl;
            }
            memset(receiveMsg, '\0', sizeof(receiveMsg));
        } // exit while
        memset(msg, '\0', sizeof(msg));                                       //reset msg
    } else {
        std::cout << "Fehler in der Connect" << std::endl;
        if (close(clientSocket) == -1){
            std::cout << "Fehler in der Close" << std::endl;                    //closesocket
        }
    }
}

