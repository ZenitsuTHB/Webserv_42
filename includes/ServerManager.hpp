/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:00:00 by avolcy            #+#    #+#             */
/*   Updated: 2025/06/07 14:30:42 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <vector>
#include <map>
#include <sys/epoll.h>
#include <csignal>
#include "Server.hpp"
#include "ServerConfig.hpp"
#include "core/ScopedPtr.tpp"
#include "sstream"
#include "core/ParseReqUtils.hpp"

#define MAX_EVENTS 10
#define BUFFER_SIZE 4096
extern volatile sig_atomic_t g_shouldStop;

typedef std::map<int, time_t> TimeMap;

class ServerManager {
public:

    ServerManager(const std::vector<ServerConfig>& configs);
    ~ServerManager();

    void stop();
    void run();
    
    size_t getServerCount() const { 
        return (_servers.size());
    }

private:
    int                         _epoll_fd;
    bool                        _running;
    TimeMap                     _lastActivity;
    std::vector<Server*>        _servers;
    std::map<int, Server*>      _fd_to_server;
    std::map<int, Server*>      _client_to_server;
    std::map<int, std::string>  _buffers;
    std::map<int, std::string>  _writeBuffers;
    std::map<int, bool> _pendingClose;

    // Méthodes privées
    void handleNewConnection(Server* server);
    void handleClientData(int client_fd, uint32_t events);
    void handleClientDisconnect(int client_fd);
    void addToEpoll(int fd, uint32_t events);
    void removeFromEpoll(int fd);
    void modifyEpoll(int fd, uint32_t events);
    void flushWriteBuffer(int client_fd);
    bool parseContentLength(const std::string& headers, size_t& outLength);


    enum BufferLimits {
        
        READ_BUFFER_SIZE = 8192,      // 8KB read buffer
        MAX_HEADER_SIZE = 8192,       // 8KB max for headers
        MAX_BODY_SIZE = 1024 * 1024,  // 1MB max for body
        MAX_TOTAL_REQUEST_SIZE = MAX_HEADER_SIZE + MAX_BODY_SIZE
    };

    // Interdiction de copie
    ServerManager(const ServerManager&);
    ServerManager& operator=(const ServerManager&);
};

#endif