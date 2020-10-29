//
// Created by Mario Eberth on 29.10.20.
//
#include <iostream>
#include <sys/socket.h> // socket, bind, listen, accept
#include <netinet/in.h> // IPPROTO_TCP, sockaddr_in,
// htons/ntohs, INADDR_ANY
#include <unistd.h> // close
#include <arpa/inet.h> // inet_ntop/inet_atop
#include <string.h> // strlen
#include <semaphore.h> // sem_init

#ifndef DIS_EX01_CLIENT_TCPCLIENT_H
#define DIS_EX01_CLIENT_TCPCLIENT_H

#define BUFFER_SIZE 1024

#define IPPORT (_argv[1])
#define IPADDRESS (_argv[2])

class TCPClient{
public:
    TCPClient(int _port, int _ipAddr);
    void initializeSocket();
    void startSocket();

private:
    int ipPort;
    int ipAddr;
    int clientSocket;
};

#endif //DIS_EX01_CLIENT_TCPCLIENT_H
