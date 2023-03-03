cmake_minimum_required(VERSION 3.24)
project(TCPProtocol)

set(CMAKE_CXX_STANDARD 23)

include_directories(server)
include_directories(tcp)

add_executable(TCPProtocol main.cpp server/server.cpp tcp/connection.h)
