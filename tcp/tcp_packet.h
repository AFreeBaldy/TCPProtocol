//
// Created by User on 3/3/2023.
//

#ifndef TCPPROTOCOL_TCP_PACKET_H
#define TCPPROTOCOL_TCP_PACKET_H



struct TCP_Header_Fixed_Size {
    unsigned short sourcePort;
    unsigned short destinationPort;
    unsigned int sequenceNumber;
    unsigned int acknowledgementNumber;
    // Contains the bits for Data Offset, Reserved Bits and the Control Bits
    unsigned short infoBits;
    unsigned short windowSize;
    unsigned short checksum;
    unsigned short urgentPointer;
};

struct TCP_Header {
    TCP_Header_Fixed_Size headerFixedSize;
    unsigned int options[10];
};

struct TCP_Packet {
    TCP_Header tcpHeader;
    char data[];
};

#endif //TCPPROTOCOL_TCP_PACKET_H
