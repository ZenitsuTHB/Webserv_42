/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 19:16:24 by avolcy            #+#    #+#             */
/*   Updated: 2025/06/02 19:33:07 by avolcy           ###   ########.fr       */
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
#include "Server.hpp"
#include "ServerConfig.hpp"

#define MAX_EVENTS 10
#define BUFFER_SIZE 4096

class ServerManager {
public:

    ServerManager(const std::vector<ServerConfig>& configs);
    ~ServerManager();

    void stop();
    void run();

private:
    int                         _epoll_fd;
    bool                        _running;
    std::vector<Server*>        _servers;
    std::map<int, Server*>      _client_to_server;
    std::map<int, std::string>  _buffers;
    // Méthodes privées
    void handleNewConnection(Server* server);
    void handleClientData(int client_fd, uint32_t events);
    void handleClientDisconnect(int client_fd);
    void addToEpoll(int fd, Server* server, uint32_t events);
    void removeFromEpoll(int fd);

    // Interdiction de copie
    ServerManager(const ServerManager&);
    ServerManager& operator=(const ServerManager&);
};

#endif