/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server2.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:55:48 by avolcy            #+#    #+#             */
/*   Updated: 2025/04/02 18:43:55 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <poll.h>
# include <fcntl.h>
# include <stdio.h>
# include <fcntl.h>
# include <iostream>
# include <algorithm>
# include <sys/errno.h>
# include "../includes/Server.hpp"


Server::~Server(void)
{
}

void           Server::setNonBlocking( int socketFd, bool enable)
{
        int flags = fcntl( socketFd, F_GETFL, 0 );
	if ( flags  == -1 )
	{
		_socket.close();
		return (perror( "<SERVER> fcntl failed !"));
	}	
	if ( enable )
        {
                if ( fcntl( socketFd, F_SETFL, flags | O_NONBLOCK ) == -1 )
		{	_socket.close();
			return ( perror("<SERVER> Error Activating O_NONBLOCK"));
        	}
	}
	else
	{
		if ( fcntl(_sockFd, F_SETFL, flags & ~O_NONBLOCK) == -1 )
		{
			_socket.close();
			return ( perror("<SERVER> Error deActivating O_NONBLOCK"));
		}
	}
}

Server::Server(int domain, int type, int protocol):
        _socket(domain, type, protocol)
{
        int     flags;
        int     fd;
        int     opt;

        fd = _socket.getSockFd();
        opt = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        {
                _socket.close();
               return ( perror("<Server> Error in opt"));
                
        }

	_socket.setNonBlocking( fd, true );

        _clientfd = -1;
        _epoll_fd = epoll_create1( 0 );
        if ( _epoll_fd == -1 )
		return (perror("<SERVER> Failed to create epoll instance"));
        return ;
}
../includes/Server.hpp
Server::Server(Server const &obj):
        _socket(obj._socket)
{
        *this = obj;
}

void    Server::close( int fd )
{
    if ( clientsMap.find( fd ) == clientsMap.end() ) return;
    clientsMap[fd].close();
    epoll_ctl( _epoll_fd, EPOLL_CTL_DEL, fd, NULL );
    clientsMap.erase( fd );
}

void    Server::start( int ip, int port, int backlog )
{
        _socket.bind( ip, port );
        _socket.listen( backlog );
        _socket.setNonBlocking( true );

        struct  epoll_event event;
        event.events = EPOLLIN | EPOLLET;
        event.data.fd = _socket.getSockFd();
        if( epoll_ctl( _epoll_fd, EPOLL_CTL_ADD, _socket.getSockFd(), &event) == -1 )
                throw std::runtime_error( "<SERVER> Failed to add socket to epoll: " +
                                std::string(strerror(errno)));
        _running = true;
        std::cout << "<SERVER> started on ip : [ " << ip << " ] and port : [ " << port << " ]\n"
}

void	Server::acceptNewConnection( void )
{
    while (true)
    {
        BaseSocket clientSocket = _socket.accept();
        if ( clientSocket.getSockFd() == -1)
        {
                if ( errno == EAGAIN || errno == EWOULDBLOCK )
                        break;
                else
                        return( perror( "<Server> Error accepting client" ));
        }

        clientsMap[clientSocket.getSockFd()] = clientSocket;
	clientSocket.setNonBlocking( true );

        struct epoll_event event;

        event.events = EPOLLIN | EPOLLRDHUP | EPOLLET | EPOLLHUP;
        event.data.fd = clientSocket.getSockFd();

        if ( epoll_ctl( _epoll_fd, EPOLL_CTL_ADD, clientSocket.getSockFd(), &event ) == -1 )
        {
            perror( "<Server> Failed to add client to epoll" );
            clientSocket.close( clientSocket.getSockFd() );
        }
    }
    return ;
}

void	Seerver::run( void )
{
	while ( _running )
	{
		int eventCounter = epoll_wait( _epoll_fd, _events, MAX_EVENTS, -1);
		if ( eventCounter == -1 )
		{
			if( errno == EINTR ) continue;
			throw std::runtime_error( "<SERVER> epoll_wait failed " + std::string(strerror( errno )));
		}
		
		for ( int i = 0; i < eventCounter; i++)
		{
			int clientFd = _events[i].data.fd;//new client
			if ( eventFd == _socket.getSockFd() )
                                acceptNewConnection();
                        else
                                handleClientEvent( clientFd, _events );//manage connected client
		}

	}
}

int	main() 
{
}
