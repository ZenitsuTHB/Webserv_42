/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 18:06:07 by avolcy            #+#    #+#             */
/*   Updated: 2025/06/02 18:34:41 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include <fstream>
#include <sstream>
#include <errno.h>
#include <stdexcept>

Server::Server(in_addr_t ip, in_port_t port, int backlog, int epoll_fd)
    : _socket(AF_INET, SOCK_STREAM, 0), _buffer(BUFF_SIZE)
{
    (void)epoll_fd;
    int fd = _socket.getSockFd();
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        throw std::runtime_error("<Server> setsockopt(SO_REUSEADDR) failed");

    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1 || fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
        throw std::runtime_error("<Server> fcntl non-blocking failed");

    _socket.bind(ip, port);
    _socket.listen(backlog);

    _clientfd = -1;
    _responseReady = false;
    prepareStaticResponse();
}

Server::~Server(void) {}

int Server::getSocketFd() const {
    return _socket.getSockFd();
}

int Server::acceptConnection() {
    BaseSocket clientSocket = _socket.accept();
    if (clientSocket.getSockFd() == -1)
        return -1;

    int fd = clientSocket.getSockFd();
    _clientsMap[fd] = clientSocket;

    int flags = fcntl(fd, F_GETFL, 0);
    if (flags != -1)
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    return fd;
}

std::string Server::processRequest(const std::string& request) {
    (void)request;
    if (!_responseReady)
        return "HTTP/1.1 404 Not Found\r\nContent-Length:13\r\n\r\n404 Not Found";

    return _cachedResponse;
}

void Server::prepareStaticResponse() {
    std::ifstream file("./html/index.html");
    if (!file) {
        std::cerr << "<SERVER> Failed to load index.html\n";
        _responseReady = false;
        return;
    }

    std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "
             << body.size() << "\r\n\r\n" << body;

    _cachedResponse = response.str();
    _responseReady = true;
}
