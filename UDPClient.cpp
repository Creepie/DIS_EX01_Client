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

}

void UDPClient::startSocket() {

}