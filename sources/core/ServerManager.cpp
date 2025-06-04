/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:22:24 by avolcy            #+#    #+#             */
/*   Updated: 2025/06/04 15:51:27 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <map>
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
            addToEpoll(server->getSocketFd(), EPOLLIN);
            _fd_to_server[server->getSocketFd()] = server;
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
    _fd_to_server.clear();
    _client_to_server.clear();
    _buffers.clear();
    _writeBuffers.clear();

    if (_epoll_fd != -1) {
        close(_epoll_fd);
        _epoll_fd = -1;
    }
    _running = false;
}

void ServerManager::run() {
    
    struct epoll_event events[MAX_EVENTS];
    while (_running) {
        time_t now = time(NULL);
        for (TimeMap::iterator it = _lastActivity.begin(); it != _lastActivity.end(); ) {
            if (now - it->second > 30) { // 30 seconds timeout
                std::cout << "[TIMEOUT] Closing idle client: " << it->first << std::endl;
                handleClientDisconnect(it->first);
                _lastActivity.erase(it++);
            } else {
                ++it;
            }
        }

        int nfds = epoll_wait(_epoll_fd, events, MAX_EVENTS, 1000);
        if (nfds == -1) {
            if (errno == EINTR) continue;
            throw std::runtime_error(std::string("epoll_wait: ") + strerror(errno));
        }

        for (int i = 0; i < nfds; ++i) {
            int fd = events[i].data.fd;

            if (_client_to_server.find(fd) != _client_to_server.end()) {
                _lastActivity[fd] = now;
                if (events[i].events & EPOLLOUT) {
                    flushWriteBuffer(fd);
                } else {
                    handleClientData(fd, events[i].events);
                }
            } else if (_fd_to_server.find(fd) != _fd_to_server.end()) {
                handleNewConnection(_fd_to_server[fd]);
            } else {
                std::cerr << "[ServerManager] Unknown fd triggered epoll: " << fd << std::endl;
            }
            if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                handleClientDisconnect(fd);
            }
        }
    }
}


void ServerManager::handleNewConnection(Server* server) {
    int client_fd = server->acceptConnection();
    if (client_fd == -1) return;

    _client_to_server[client_fd] = server;
    addToEpoll(client_fd, EPOLLIN | EPOLLET | EPOLLRDHUP);
}

void ServerManager::handleClientData(int client_fd, uint32_t events) {
    (void)events;
    Server* server = _client_to_server[client_fd];
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    std::string& clientBuffer = _buffers[client_fd];

    while ((bytes_read = read(client_fd, buffer, sizeof(buffer)))) {
        if (bytes_read <= 0) break;
        clientBuffer.append(buffer, bytes_read);

        size_t headerEnd = clientBuffer.find(END);
        if (headerEnd != std::string::npos) {
            std::string request = clientBuffer.substr(0, headerEnd + 4);
            std::string response = server->processRequest(request);

            _writeBuffers[client_fd] = response;
            modifyEpoll(client_fd, EPOLLOUT | EPOLLET | EPOLLRDHUP);

            _buffers[client_fd].erase(0, headerEnd + 4);
            break;
        }
    }
}

void ServerManager::flushWriteBuffer(int client_fd) {
    std::map<int, std::string>::iterator it = _writeBuffers.find(client_fd);
    if (it == _writeBuffers.end()) return;

    const std::string& response = it->second;
    ssize_t sent = send(client_fd, response.c_str(), response.size(), 0);

    if (sent == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            handleClientDisconnect(client_fd);
        }
        return;
    }

    if ((size_t)sent < response.size()) {
        _writeBuffers[client_fd] = response.substr(sent);
    } else {
        _writeBuffers.erase(client_fd);

        if (_buffers[client_fd].find(END) != std::string::npos) {
            modifyEpoll(client_fd, EPOLLIN | EPOLLET | EPOLLRDHUP);
        } else {
            modifyEpoll(client_fd, EPOLLIN | EPOLLET | EPOLLRDHUP);
        }
    }
}

void ServerManager::handleClientDisconnect(int client_fd) {
    removeFromEpoll(client_fd);
    _client_to_server.erase(client_fd);
    _buffers.erase(client_fd);
    _writeBuffers.erase(client_fd);
    close(client_fd);
}

void ServerManager::addToEpoll(int fd, uint32_t events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        throw std::runtime_error(std::string("epoll_ctl ADD: ") + strerror(errno));
    }
}

void ServerManager::modifyEpoll(int fd, uint32_t events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, fd, &ev) == -1) {
        std::cerr << "Warning: epoll_ctl MOD failed: " << strerror(errno) << std::endl;
    }
}

void ServerManager::removeFromEpoll(int fd) {
    if (fcntl(fd, F_GETFD) == -1 && errno == EBADF) return; // Already closed
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        std::cerr << "Warning: epoll_ctl DEL failed: " << strerror(errno) << std::endl;
    }
}


ServerManager::~ServerManager() {
    stop();
}
