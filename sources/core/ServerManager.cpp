/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:22:24 by avolcy            #+#    #+#             */
/*   Updated: 2025/06/02 19:37:10 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include "../../includes/ServerManager.hpp"


ServerManager::ServerManager(const std::vector<ServerConfig>& configs) : _epoll_fd(-1), _running(false) {
    
    _epoll_fd = epoll_create1(0);
    if (_epoll_fd == -1) {
        throw std::runtime_error(std::string("epoll_create1: ") + strerror(errno));
    }

    for (std::vector<ServerConfig>::const_iterator it = configs.begin(); it != configs.end(); ++it) {
        Server* server = NULL;
    
        try {
            server = new Server(it->getIp(), it->getPort(), it->getBacklog(), _epoll_fd);
            if (server->getSocketFd() == -1) {
                delete server;
                continue;
            }
            addToEpoll(server->getSocketFd(), server, EPOLLIN);
            _servers.push_back(server);
        } catch (const std::exception& e) {
            std::cerr << "[ServerManager] Failed to initialize server: " << e.what() << std::endl;
            delete server;
        }
    }
    
    _running = true;
}

void ServerManager::stop() {
    if (!_running) return;

    for (std::vector<Server*>::iterator it = _servers.begin(); 
         it != _servers.end(); ++it) {
        delete *it;
    }
    _servers.clear();

    if (_epoll_fd != -1) {
        close(_epoll_fd);
        _epoll_fd = -1;
    }
    _running = false;
}

void ServerManager::run() {
    
    struct epoll_event events[MAX_EVENTS];
    while (_running) {
        int nfds = epoll_wait(_epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            if (errno == EINTR) continue;
            throw std::runtime_error(std::string("epoll_wait: ") + strerror(errno));
        }

        for (int i = 0; i < nfds; ++i) {
            Server* server = static_cast<Server*>(events[i].data.ptr);
            std::cout <<"I just create a server \n";
            int fd = server->getSocketFd();

            if (events[i].events & EPOLLIN) {
                if (_client_to_server.find(fd) == _client_to_server.end()) {
                    // Nouvelle connexion sur un socket serveur
                    handleNewConnection(server);
                } else {
                    // Données sur un socket client
                    handleClientData(fd, events[i].events);
                }
            } else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                handleClientDisconnect(fd);
            }
        }
    }
}

void ServerManager::handleNewConnection(Server* server) 
{
    int client_fd = server->acceptConnection();
    if (client_fd == -1) return;

    _client_to_server[client_fd] = server;
    addToEpoll(client_fd, server, EPOLLIN | EPOLLET | EPOLLRDHUP);
}

// void ServerManager::handleClientData(int fd, uint32_t events) {
//     _client_to_server[fd]->handleClientEvent(fd, events, _epoll_fd);
// }

void ServerManager::handleClientData(int client_fd, uint32_t events) {
    (void)events;
    Server* server = _client_to_server[client_fd];
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    std::string& clientBuffer = _buffers[client_fd]; // Add this map: std::map<int, std::string> _buffers;

    while ((bytes_read = read(client_fd, buffer, sizeof(buffer)))) {
        if (bytes_read <= 0) break;
        clientBuffer.append(buffer, bytes_read);

        // Check for end of headers
        size_t headerEnd = clientBuffer.find(END);
        if (headerEnd != std::string::npos) {
            std::string request = clientBuffer.substr(0, headerEnd + 4);
            std::string response = server->processRequest(request);

            send(client_fd, response.c_str(), response.size(), 0);
            _buffers[client_fd].clear();

            // Optional: close if `Connection: close`
            handleClientDisconnect(client_fd);
            break;
        }
    }
}


void ServerManager::handleClientDisconnect(int client_fd) {
    removeFromEpoll(client_fd);
    _client_to_server.erase(client_fd);
    close(client_fd);
}

void ServerManager::addToEpoll(int fd, Server* server, uint32_t events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = server;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        throw std::runtime_error(std::string("epoll_ctl ADD: ") + strerror(errno));
    }
}

void ServerManager::removeFromEpoll(int fd) {
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        std::cerr << "Warning: epoll_ctl DEL failed: " << strerror(errno) << std::endl;
    }
}

ServerManager::~ServerManager() {
    stop();
}
