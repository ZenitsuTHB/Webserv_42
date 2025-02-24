#include "server.hpp"

SimpleSocket::SimpleSocket(int domain, int type, int protocol, int port, u_long interface)
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

void    SimpleSocket::testConnection(int itemTotest)
{
    if (itemTotest < 0)
    {
        //std::cerr << "Error: " << strerror(errno) << std::endl;
	perror("Failed to connect...!");
        exit(EXIT_FAILURE);
    }
}

SimpleSocket::~SimpleSocket()
{
}


