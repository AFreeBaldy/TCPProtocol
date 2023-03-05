cmake_minimum_required(VERSION 3.24)
project(TCPProtocol)

set(CMAKE_CXX_STANDARD 23)

include_directories(server)
include_directories(tcp)

add_executable(TCPProtocol main.cpp server/server.cpp tcp/tcp_connection.h tcp/tcp_packet.h tcp/tcp_windows_include.h tcp/tcp_unix_include.h tcp/tcp_windows_connection.h)
if (WIN32)
    target_link_libraries(TCPProtocol ws2_32)
endif ()

