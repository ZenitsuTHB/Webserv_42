/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server2.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:55:48 by avolcy            #+#    #+#             */
/*   Updated: 2025/04/04 17:30:37 by avolcy           ###   ########.fr       */
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

# define LOG( msg ) std::cout << "[SERVER]" << msg << std::endl

Server::~Server(void)
{
}

void           Server::setNonBlocking( int socketFd, bool enable )
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
		{
			_socket.close();
			return ( perror("<SERVER> Error Activating O_NONBLOCK"));
        	}
	}
	else
	{
		if ( fcntl( socketFd, F_SETFL, flags & ~O_NONBLOCK) == -1 )
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
		throw std::runtime_error("<Server> setsockopt(SO_REUSEADDR) failed");
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

void	Server::shutDownServer()
{
	std::map< int, BaseSocket >::iterator it;
        _running = false;
	for ( it = _clientsMap.begin(); it != _clientsMap.end(); ++it )
		it->second.close();
	close( _epoll_fd );
	_socket.close();
	std::cout << "<SERVER> Shutdown complete." << std::endl;
}

void    Server::closeClient( int fd )
{
	std::map< int, BaseSocket >::iterator it = _clientsMap.find( fd ); 
	if ( it == _clientsMap.end() ) return;
	
	it->second.close();
	if ( epoll_ctl( _epoll_fd, EPOLL_CTL_DEL, fd, NULL ) == -1)
	        perror("<SERVER ERR removing client from epoll>");
	_clientsMap.erase( it );
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

        _clientsMap[clientSocket.getSockFd()] = clientSocket;
	clientSocket.setNonBlocking( clientSocket.getsockFd(), true );
        //_socket.getSockFd() fd du Server

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

// EPOLLHUP the connection get entirely cut off
// EPOLLRDHUP the client shut its part using close or 
// shutdown(fd , SHUT_WR) and it can still receive data
void	Server::handleClientEvent( int fdClient, uint32_t events )
{
	if ( events & ( EPOLLRDHUP | EPOLLHUP | EPOLLERR ))
	{
		if ( events & EPOLLRDHUP )
		{
			std::cout << "<SERVER> Client has been disconnected : [ " << fdClient << " ] gracefully" << std::endl;
			// read the remain data
			char tmpBuf[1024];
			while (read(fdClient, tmpBuf, sizeof(tmpBuf)) > 0) {}//empty the buffer
			closeClient( fdClient );
			return ;
		}
                if ( events & EPOLLHUP )
                {
                        std::cout << "<SERVER> Client has been disconnected : [ " << fdClient << " ] abrutptly" << std::endl;
                        closeClient( fdClient );
                        return ;
                }
		closeClient( fdClient );
                return ;
	}
	if ( events & EPOLLIN)
	{
		ssize_t bytesRead = -1;
		while( (bytesRead = read( fdClient, _buffer.data() , _buffer.size() - 1)) > 0 )
		{
			_buffer[bytesRead] = '\0';
                        
                        LOG("Read from client " << fdClient << ": " << _buffer.data());
			//str::response = function for reponse/TODO
			std::string response = "HTTP/1.1 200 OK\r\nContent-Length:20\r\n\r\nHello Tela SERVIDA !";
			ssize_t totalSent = 0;
                        const char *ptr = response.c_str();
                        while ( totalSent < response.size() )
                        {
                                ssize_t sent = write(fdClient, ptr + totalSent, response.size() - totalSent);
                                if (sent == -1)
                                {
                                        if (errno == EAGAIN || errno == EWOULDBLOCK)//cpntinue; ?
                                                break;
                                        else
                                        {
                                                std::cerr << "Write error to client " << fdClient << ": " << strerror(errno) << std::endl;
                                                closeClient(fdClient);
                                                return;
                                        }
                                }
                                totalSent += sent;
                        }
		}
		if (bytesRead == -1 && errno != EAGAIN)
		{
                        LOG("Read error for fd: " << fdClient << strerror(errno));
                        closeClient(fdClient);
		}
        }
}

void	Server::run( void )
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
			if ( clientFd == _socket.getSockFd() )
                                acceptNewConnection();
                        else
                                handleClientEvent( clientFd, _events );//manage connected client
		}

	}
}

int	main() 
{
}
