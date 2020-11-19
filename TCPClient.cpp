//
// Created by Mario Eberth on 29.10.20.
//

#include "TCPClient.h"

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
 * @param _port is the given port
 * @param _ipAddr is the given ipAddress
 */
TCPClient::TCPClient(int _port, int _ipAddr) {
    //global variable for the ipPort
    ipPort = _port;
    //global variable for the ipAddress
    ipAddr = _ipAddr;
}

/**
 * this method initialize the clientSocket
 */
void TCPClient::initializeSocket() {

    /**
    * Socket > initialize variables for creating the clientSocket
    */
    int mAddressFormat = AF_INET;                                                   //Format Ipv4
    int mSocketType = SOCK_STREAM;                                                  //TCP
    int mSocketProtocol = 0;                                                        //communication protocol > self check

    clientSocket = socket(mAddressFormat, mSocketType, mSocketProtocol);    //create the client socket
}

/**
 * in this method the socket connect to the server > after that the client communicate with the server
 */
void TCPClient::startSocket() {

    /**
     * Connect
     * add data into mServerAddr > we need it for the connect
     */
    sockaddr_in mServerAddr;                                //save mServerAddr in it
    mServerAddr.sin_family = AF_INET;                       //Format Ipv4
    mServerAddr.sin_port = ipPort;                          //get the Port from the IPPORT (htons = host to network short, athoi = argument to integer)
    mServerAddr.sin_addr.s_addr = ipAddr;                   //get the IpAdress from IPADDRESS
    memset(&(mServerAddr.sin_zero), '\0', 8);         // \0 get copied in the first 8 char character of sin_zero //https://www.geeksforgeeks.org/memset-in-cpp/

    /**
     * check is connection successful > errorCode >= 0
     */
    if (connect(clientSocket, (sockaddr *) &mServerAddr, sizeof(mServerAddr)) >= 0) {
        char mMsg[BUFFER_SIZE];                                                  //creates a charArray
        memset(mMsg, '\0' , sizeof(mMsg));
        while (strcmp(mMsg, "exit") != 0 && strcmp(mMsg, "shutdown") != 0) {
            /**
             * send message and check the return val of the method ( >0 means no error)
             */
            std::cout << "Bitte um Eingabe: ";
            std::cin.getline(mMsg, BUFFER_SIZE);                                     //get the input line and save it into mMsg
            strcat(mMsg, "\0");
            int mMsgSize = strlen(mMsg) + 1;                                          //get the length of the mMsg
            //send
            if (!send(clientSocket, mMsg, mMsgSize, 0) > 0) {                          //send the message and check the return value of the send method
                std::cout << "Error Sending message" << std::endl;
                return;
            }

            /**
             * receive the message and check the return val of the method ( >0 means no error)
             */
            char mReceiveMsg[BUFFER_SIZE];                                               //create a char array
            if (recv(clientSocket, mReceiveMsg, BUFFER_SIZE, 0) > 0) {                    //recv the message and check the error code (return value)
                std::cout << mReceiveMsg;
            }else{
                std::cout << "Error mReceiveMsg" << std::endl;
                return;
            }
            //reset the mMsg
            memset(mReceiveMsg, '\0', sizeof(mReceiveMsg));
        } // exit while
        memset(mMsg, '\0', sizeof(mMsg));                                       //reset mMsg
    } else {
        std::cout << "Fehler in der Connect" << std::endl;
        if (close(clientSocket) == -1){
            std::cout << "Fehler in der Close" << std::endl;                    //closesocket
        }
    }
} //end of startSocket method

