/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:24:59 by avolcy            #+#    #+#             */
/*   Updated: 2025/05/21 18:25:05 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ======================
// 📁 ClientManager.cpp
// ======================
#include "ClientManager.hpp"
#include <unistd.h>
#include <sys/epoll.h>
#include <iostream>

void ClientManager::addClient(int fd, const BaseSocket& socket) {
    _clients[fd] = socket;
    _clientBuffers[fd] = "";
}

void ClientManager::removeClient(int fd, int epollFd) {
    if (epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL) == -1 && errno != EBADF && errno != ENOENT) {
        perror("[ClientManager] epoll_ctl DEL error");
    }
    close(fd);
    _clients.erase(fd);
    _clientBuffers.erase(fd);
}

bool ClientManager::clientExists(int fd) const {
    return _clients.find(fd) != _clients.end();
}

void ClientManager::updateClientBuffer(int fd, const std::string& data) {
    _clientBuffers[fd] += data;
}

void ClientManager::clearClientBuffer(int fd) {
    _clientBuffers[fd].clear();
}

const std::string& ClientManager::getClientBuffer(int fd) const {
    std::map<int, std::string>::const_iterator it = _clientBuffers.find(fd);
    if (it != _clientBuffers.end())
        return it->second;
    static const std::string empty = "";
    return empty;
}

const std::map<int, BaseSocket>& ClientManager::getClients() const {
    return _clients;
}

std::map<int, std::string>& ClientManager::getClientBuffers() {
    return _clientBuffers;
}
