//
// Created by Laud on 3/4/23.
//

#ifndef TCPPROTOCOL_TCP_UNIX_INCLUDE_H
#define TCPPROTOCOL_TCP_UNIX_INCLUDE_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define closesocket(int) close(int)

#endif //TCPPROTOCOL_TCP_UNIX_INCLUDE_H
