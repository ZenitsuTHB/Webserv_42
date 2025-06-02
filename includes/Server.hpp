/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:55:19 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/06/02 18:53:48 by avolcy           ###   ########.fr       */
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
        Server(in_addr_t ip, in_port_t port, int backlog, int epoll_fd);
        ~Server();
    
        int getSocketFd() const;
        int acceptConnection();
        std::string processRequest(const std::string& request);
        void prepareStaticResponse();
    
    private:
        std::string _cachedResponse;
        std::map<int, std::string> _recvBuffers;
        bool _responseReady;
    
        ListenSocket _socket;
        int _clientfd;
        std::map<int, BaseSocket> _clientsMap;
        std::vector<char> _buffer;
    };

#endif
