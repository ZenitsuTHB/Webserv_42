#pragma once

#include <iostream>
#include <netinet/in.h>
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
#endif

class simpleSocket
{
    private:
        int connection;
        int sock;
        struct sockaddr_in address;

    public:
        simpleSocket(int domain, int type, int protocol, int port, u_long interface);
        virtual void connectToNetwork(int sock, struct sockaddr_in address) = 0;
        void testConnection(int );
        struct sockaddr_in getAddress() { return address; }
        int getSocket() { return sock; }
        int getConnection() { return connection; }
        ~simpleSocket();
};

simpleSocket::simpleSocket(int domain, int type, int protocol, int port, u_long interface)
{
    // Define address structure
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    {/*s.sin_addr.s_addr = INADDR_ANY;*/}
    // Establish connection
    sock = socket(domain, type, protocol);
    testConnection(sock);
    // Connect to network
    connection = connectToNetwork(sock, address);
    testConnection(connection);
}

void    simpleSocket::testConnection(int itemTotest)
{
    if (itemTotest == -1)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }
}

simpleSocket::~simpleSocket()
{
}
