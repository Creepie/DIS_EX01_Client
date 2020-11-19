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

/**
 * the Constructor save the given parm into the global params
 * @param _port represent the given port
 * @param _ipAddr reoresent the given ipAddress
 */
UDPClient::UDPClient(int _port, int _ipAddr) {
    //global variable for the ipPort
    ipPort = _port;
    //global variable for the ipAddress
    ipAddr = _ipAddr;
}

/**
 * this method initialize the Socket
 */
void UDPClient::initializeSocket() {

    /**
     * Socket > initialize variables for creating the clientSocket
     */
    int mAddressFormat = AF_INET;                                                   //Format Ipv4
    int mSocketType = SOCK_DGRAM;                                                  //UDP
    int mSocketProtocol = 0;                                                        //communication protocol > self check

    //clientSocket is a global variable of the socket
    clientSocket =  socket(mAddressFormat, mSocketType, mSocketProtocol);
}

/**
 * in this method the socket connect to the server > after that the client communicate with the server
 * (Server only gives an ECHO replay)
 */
void UDPClient::startSocket() {
    std::cout << "Ready to write" << std::endl;

    /**
     * save the from data (in this case we dont need it because we always talk to the same server)
     */
    sockaddr_in from;
    socklen_t frommSize = sizeof(from);

    /**
     * set toAddr (server)
     */
    sockaddr_in toAddr;
    toAddr.sin_family = AF_INET;
    toAddr.sin_port = ipPort;
    toAddr.sin_addr.s_addr = ipAddr;
    memset(&(toAddr.sin_zero),'\0',0);
    int toSize = sizeof(toAddr);

    char mMsg[BUFFER_SIZE];
    memset(mMsg, '\0' , sizeof(mMsg));
    while (strcmp(mMsg, "exit") != 0 && strcmp(mMsg, "shutdown") != 0){
        /**
         * Send and check the return value of the sendTo method (if -1 we had an error)
         */
        std::cout << "Bitte um Eingabe: ";
        std::cin.getline(mMsg, BUFFER_SIZE);                                     //get the input line and save it into mMsg
        strcat(mMsg, "\0");
        int msgSize = strlen(mMsg) + 1;

        if (sendto(clientSocket, mMsg, msgSize, 0, (sockaddr*) &toAddr, toSize) == -1){
            std::cout << "Fehler in der Send" << std::endl;
            return;
        }

        /**
         * recvfrom
         * get the ECHO from the server and check the return value ( >= 0 if no error)
         */
        char mReceiveMsg[BUFFER_SIZE];
        if (recvfrom(clientSocket, mReceiveMsg, BUFFER_SIZE, 0, (sockaddr*) &from, &frommSize) >= 0 ){
            std::cout << mReceiveMsg << std::endl;
        } else{
            return;
        }
    } //end while
    if (close(clientSocket) == -1){
        std::cout << "Fehler in der Close" << std::endl;  //closesocket
        exit(-1);
    }
}// end of startSocket method