cmake_minimum_required(VERSION 3.17)
project(DIS_EX01_Client)

set(CMAKE_CXX_STANDARD 14)

add_executable(DIS_EX01_Client main.cpp TCPClient.h TCPClient.cpp UDPClient.h UDPClient.cpp)