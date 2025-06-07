/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:55:19 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/06/07 11:58:49 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <cstring>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <vector>
# include "ListenSocket.hpp"
# include <sys/epoll.h>
# include <map>
# include <cstddef>
# include <cstdlib>


static const size_t BUFF_SIZE = 42;
# define PORT 8080
# define BACKLOG 10
# define END "\r\n\r\n"

class ListenSocket;

class Server {
    public:
        Server(in_addr_t ip, in_port_t port, int backlog, int epoll_fd, const ServerConfig& configs);
        ~Server();
    
        int getSocketFd() const;
        int acceptConnection();
        std::string processRequest(const std::string& request);
        void prepareStaticResponse();
        const ServerConfig& getConfig() const { return _configs; }
    
    private:

        ServerConfig    _configs;
        bool                        _responseReady;
        std::string                 _cachedResponse;
        std::map<int, std::string>  _recvBuffers;
        int _clientfd;
        ListenSocket _socket;
        std::vector<char> _buffer;
        std::map<int, BaseSocket> _clientsMap;
    };

#endif
