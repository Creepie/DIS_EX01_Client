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

int main(int _argc, char **_argv) {

    //Socket
    int _addressFormat = AF_INET;                                                   //Format Ipv4
    int _socketType = SOCK_STREAM;                                                  //TCP
    int _socketProtocol = 0;                                                        //communication protocol > self check

    int clientSocket = socket(_addressFormat, _socketType, _socketProtocol);    //create the client socket
    //connect
    sockaddr_in serverAddr;                                                     //
    serverAddr.sin_family = AF_INET;                                            //Format Ipv4
    serverAddr.sin_port = htons(atoi(IPPORT));                               //get the Port from the IPPORT (htons = host to network short, athoi = argument to integer)
    serverAddr.sin_addr.s_addr = inet_addr(IPADDRESS);                          //get the IpAdress from IPADDRESS
    memset(&(serverAddr.sin_zero), '\0',8);                             // \0 get copied in the first 8 char character of sin_zero //https://www.geeksforgeeks.org/memset-in-cpp/

    if (connect(clientSocket, (sockaddr *) &serverAddr, sizeof(serverAddr)) >=
        0) {   //check is connection successful > errorCode >= 0
        char msg[BUFFER_SIZE];                                                  //creates a charArray
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

#endif
