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

/**
 * the Constructor save the given parm into the global params
 * @param _port represent the given port
 * @param _ipAddr represent the given ipAddress
 */
TCPv6Client::TCPv6Client(int _port, int _ipAddr) {
    //global variable for the ipPort
    ipPort = _port;
    //global variable for the ipAddress
    ipAddr = _ipAddr;
}

/**
 * this method initialize the clientSocket
 */
void TCPv6Client::initializeSocket() {

    /**
    * Socket > initialize variables for creating the clientSocket
    */
    int mAddressFormat = AF_INET6;                                                   //Format Ipv6
    int mSocketType = SOCK_STREAM;                                                  //TCP
    int mSocketProtocol = 0;                                                        //communication protocol > self check

    //save the created socket into global variable clientSocket
    clientSocket = socket(mAddressFormat, mSocketType, mSocketProtocol);        //create the client socket
}

/**
 * in this method the socket connect to the server and write to the server
 */
void TCPv6Client::startSocket() {
    /**
     * connect
     * add data into mServerAddr > we need it for the connect
     */
    struct sockaddr_in6 mServerAddr;
    mServerAddr.sin6_family = AF_INET6;
    mServerAddr.sin6_flowinfo = 0;
    mServerAddr.sin6_port = ipPort;
    mServerAddr.sin6_scope_id = 0;
    /**
     * convert the ipv4 into ipv6
     */
    if (inet_pton(AF_INET6, "::1", &(mServerAddr.sin6_addr)) < 1){
        std::cout << "Fehler bei convert" << std::endl;
        return;
    }

    /**
     * connect client with server and check return value ( >= 0 means no error)
     */
    if (connect(clientSocket, (sockaddr *) &mServerAddr, sizeof(mServerAddr)) >= 0) {
        char mMsg[BUFFER_SIZE];                                                      //creates a charArray
        memset(mMsg, '\0' , sizeof(mMsg));
        while (strcmp(mMsg, "exit") != 0 && strcmp(mMsg, "shutdown") != 0) {
            //read message
            std::cout << "Bitte um Eingabe: ";
            std::cin.getline(mMsg, BUFFER_SIZE);                                     //get the input line and save it into mMsg
            strcat(mMsg, "\0");
            int msgSize = strlen(mMsg) + 1;                                          //get the length of the mMsg
            /**
             * send the message and check the return val ( >0 means no error)
             */
            if (!send(clientSocket, mMsg, msgSize, 0) > 0) {                          //send the message and check the return value of the send method
                std::cout << "Error Sending message" << std::endl;
                return;
            }

            /**
             * receive the message and check the return value ( >0 means no error)
             */
            char mReceiveMsg[BUFFER_SIZE];                                               //create a char array
            if (recv(clientSocket, mReceiveMsg, BUFFER_SIZE, 0) > 0) {                    //recv the message and check the error code (return value)
                std::cout << mReceiveMsg << std::endl;
            } else{
                std::cout << "Error mReceiveMsg" << std::endl;
                return;
            }
            //reset the message
            memset(mReceiveMsg, '\0', sizeof(mReceiveMsg));
        } // exit while
        memset(mMsg, '\0', sizeof(mMsg));                                       //reset mMsg
    } else {
        std::cout << "Fehler in der Connect" << std::endl;
        if (close(clientSocket) == -1){
            std::cout << "Fehler in der Close" << std::endl;                    //closesocket
        }
    }
} //end startSocket method

