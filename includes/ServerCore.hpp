
#ifndef SERVER_CORE_HPP
#define SERVER_CORE_HPP

#include "ListenSocket.hpp"
#include "ClientManager.hpp"
#include "RequestHandler.hpp"
#include <sys/epoll.h>

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