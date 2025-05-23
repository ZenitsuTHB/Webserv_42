/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:27:37 by avolcy            #+#    #+#             */
/*   Updated: 2025/05/23 19:10:37 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ======================
// 📁 NetworkUtils.cpp
// ======================

#include "../../includes/core/NetworkUtils.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <sys/epoll.h>

void NetworkUtils::setNonBlocking(int fd, bool enable) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        throw std::runtime_error("fcntl(F_GETFL) failed: " + std::string(strerror(errno)));

    if (enable) {
        if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
            throw std::runtime_error("fcntl(F_SETFL) O_NONBLOCK failed: " + std::string(strerror(errno)));
    } else {
        if (fcntl(fd, F_SETFL, flags & ~O_NONBLOCK) == -1)
            throw std::runtime_error("fcntl(F_SETFL) clear O_NONBLOCK failed: " + std::string(strerror(errno)));
    }
}

void NetworkUtils::closeSocket(int fd) {
    if (close(fd) == -1) {
        std::cerr << "[NetworkUtils] Error closing socket: " << strerror(errno) << std::endl;
    }
}

void NetworkUtils::epollControl(int epollFd, int op, int fd, uint32_t events) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
    if (epoll_ctl(epollFd, op, fd, &event) == -1) {
        throw std::runtime_error("epoll_ctl failed: " + std::string(strerror(errno)));
    }
}

bool NetworkUtils::isConnectionClosed(const std::string& request) {
    return request.find("Connection: close") != std::string::npos;
}