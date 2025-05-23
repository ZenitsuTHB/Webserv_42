/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCore.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:11:08 by avolcy            #+#    #+#             */
/*   Updated: 2025/05/23 18:51:09 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CORE_HPP
#define SERVER_CORE_HPP

#include "Server.hpp"
#include "../ListenSocket.hpp"
#include "ClientManager.hpp"
#include "RequestHandler.hpp"
#include <sys/epoll.h>
static const int  MAX_EVENTS  = 5;


class ServerCore {
public:
    ServerCore(in_addr_t ip, in_port_t port, int backlog);
    ~ServerCore();

    void run();
    void shutdown();

private:
    void initializeEpoll();
    void handleEvents(int timeout = -1);
    void cleanup();

    ListenSocket     _listenSocket;
    int              _epollFd;
    ClientManager    _clientManager;
    RequestHandler   _requestHandler;
    bool             _isRunning;
    epoll_event      _events[MAX_EVENTS];
};

#endif