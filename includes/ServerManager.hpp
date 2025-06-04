/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 19:16:24 by avolcy            #+#    #+#             */
/*   Updated: 2025/06/05 00:25:49 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:00:00 by avolcy            #+#    #+#             */
/*   Updated: 2025/06/03 14:00:00 by avolcy           ###   ########.fr       */
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
    size_t getServerCount() const;


private:
    int                         _epoll_fd;
    bool                        _running;
    TimeMap                     _lastActivity;
    std::vector<Server*>        _servers;
    std::map<int, Server*>      _client_to_server;
    std::map<int, Server*>      _fd_to_server;
    std::map<int, std::string>  _buffers;
    std::map<int, std::string>  _writeBuffers;
    // Méthodes privées
    void handleNewConnection(Server* server);
    void handleClientData(int client_fd, uint32_t events);
    void handleClientDisconnect(int client_fd);
    void addToEpoll(int fd, uint32_t events);
    void removeFromEpoll(int fd);
    void modifyEpoll(int fd, uint32_t events);
    void flushWriteBuffer(int client_fd);




    // Interdiction de copie
    ServerManager(const ServerManager&);
    ServerManager& operator=(const ServerManager&);
};

#endif