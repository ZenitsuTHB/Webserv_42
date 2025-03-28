/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  Server.cpp                                           :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/03/07 21:48:44 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/03/10 12:56:59 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <poll.h>
#include <algorithm>
#include "../includes/Server.hpp"

Server::~Server(void)
{
}

Server::Server(int domain, int type, int protocol):
	_socket(domain, type, protocol)
{
	int	flags;
	int	fd;
	int	opt;

	fd = _socket.getSockFd();
	opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		_socket.close();
		perror("<Server> Error in opt");
		return ;
	}
	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
	{
		_socket.close();
		perror("<Server> fcntl failed");
		return ;
	}
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) == -1)
	{
		_socket.close();
		perror("<Server> Error to add O_NONBLOCK");
	}
	_clientfd = -1;
	_epoll_fd = epoll_create1( 0 );
	if (_epoll_fd == -1)
	{
    		perror("<SERVER> Failed to create epoll instance");
    		return ;
	}

	return ;
}

Server::Server(Server const &obj):
	_socket(obj._socket)
{
	*this = obj;
}

void	Server::start(int ip, int port, int backlog)
{
	_socket.bind(ip, port);
	_socket.listen(backlog);
	_socket.setNonBlocking( true );

	struct	epoll_event event;
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = _socket.getSockFd();
	if( epoll_ctl( _epoll_fd, EPOLL_CTL_ADD, _socket.getSockFd(), &event) == -1 )
		throw std::runtime_error("<SERVER> Failed to add socket to epoll: " +
			       	std::string(strerror(errno)));
	_running = true;

	std::cout << "<SERVER> started on ip : [ " << ip << " ] and port : [ " << port << " ]\n"
}

void		acceptNewConnection( void )
{
    while (true)
    {
        BaseSocket clientSocket = _socket.accept();
        if (clientSocket.getSockFd() == -1)
	{
		if ( errno == EAGAIN || errno == EWOULDBLOCK )
                	break;
		else
			return( perror( "<Server> Error accepting client" );
        }
	


        int flags = fcntl(clientSocket.getSockFd(), F_GETFL, 0);
        if (flags == -1 || fcntl(client.getSockFd(), F_SETFL, flags | O_NONBLOCK) == -1)
	{
            	perror("<Server> Failed to set non-blocking mode");
            	clientSocket.close();
            	continue;
        }

        _clientList[client.getSockFd()] = client;

        struct epoll_event event;

        event.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
        event.data.fd = client.getSockFd();

        if ( epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client.getSockFd(), &event) == -1 ) 
	{
            perror("<Server> Failed to add client to epoll");
            client.close();
            _clientList.erase(client.getSockFd());
        }
    }
    return 0;
}

bool	endRequest(std::string message)
{
	size_t	len;

	len = message.length();
	if (len <= 3)
		return (false);
	return (message.find("\r\n\r\n", len - 4) != std::string::npos);
}

std::string	Server::receive( int idx ) const
{
	std::string	request;
	char		buffer[BUFF_SIZE + 1];
	int			bytes;

	if (idx < 0 || (size_t)idx >= _clientList.size())
		return ("<Server> Invalid Index");
	while (true)
	{
		bytes = read(_clientList[idx].getSockFd(), buffer, BUFF_SIZE );
		if (bytes == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break ;
			return ( perror( "<Server> Error reading" ), std::string() );
		}
		if ( bytes == 0 )
		{
			std::cout << "<SERVER> Client closed connection: " << _clientfd << std::endl;
			return "";
		}
			//break ;
		buffer[bytes] = '\0';
		request.append(buffer);
		if (endRequest(request))
			break ;
	}
	return (request);
}

std::string	manage(const std::string& request) const 
{
    
	if (request.find("GET") != 0 && request.find("POST") != 0 )
		return "HTTP/1.1 400 Bad Request\r\n\r\n";
	
	std::string body = "This is the Server Respond";
	std::string response = 
		"HTTP/1.1 200 OK\r\n"
        	"Content-Type: text/plain\r\n"
        	"Content-Length: " + std::to_string(body.length()) + "\r\n"
        	"Connection: close\r\n"
        	"\r\n" + body;
    
    return ( response );
}

void	Server::respond(std::string response, int idx) const
{
	int	bytes;
	
	if ( _clientList.find( fd ) == _clientList.end() ) return;
	bytes = write(_clientList[idx].getSockFd(), response.c_str(),
			response.length());
	if (bytes == -1)
		return (perror("<Server> Error responding"));
}

void	Server::close(int fd)
{
    if ( _clientList.find( fd ) == _clientList.end() ) return;
    _clientList[fd].close();
    epoll_ctl( _epoll_fd, EPOLL_CTL_DEL, fd, NULL );
    _clientList.erase( fd );
}

void	Server::run(void)
{

	while (true)
	{
		int eventCounter = epoll_wait( _epoll_fd, _events, MAX_EVENTS, -1 );
		if ( eventCounter == -1 )
		{
			if ( errno == EINTR ) continue ;
			perror( "<SERVER> Error epoll_wait\n" );
			break;
		}
		std::cout << "<SERVER> Looking for reads\n" << std::endl;
		for ( int i = 0; i < eventCounter; i++ )
		{
			int clientFd = _events[i].data.fd;

			if ( _events[i].events & ( EPOLLERR | EPOLLHUP | EPOLLRDHUP ))
			{
				std::cout << "<SERVER> Client disconnected: " << clientFd << std::endl;
				close( clientFd );
				continue;
			}

			if ( _events[i].events & EPOLLIN )
			{
				std::string request = receive( clientFd );
				if ( request.empty() )
				{
					close( clientfd );
					continue;
        	        	}
        	        	std::string response = manage( request );
        	        	respond( response, clientfd );
				
			}

		// modificar SERVER::close para que reciba un client fd para cerrar
		// y lo quita de la structura epoll
	}
}

Server	&Server:: operator = (Server const &obj)
{
	if (this != &obj)
	{
		_socket = obj._socket;
	}
	return (*this);
}


int	main(void)
{
	Server	server(AF_INET, SOCK_STREAM, 0);

	server.start(INADDR_ANY, 8080, SOMAXCONN);
	server.run();

}

// this version allows to accept multiple clients at the same time
// stock the clients in an unordered_map instead of a vector
// adding the clients immediately to epoll instead of having two seperate steps

// a line of client waiting to enter the theatre
// give a client a badge to enter 
// if current client is able to enter
	// if there's no any other client we close the door( the loop )
//check if the client is in blocking mode before giving it access to enter
// Make a client non-blocking after checking its status, so it can't block others from entering
	// if we can't make it non-blocking, we refuse to give it a pass we call the next one
//we add it to the register so we can have control over it( better is the unordered_map )
// prepare a special ticket to have a glance on it, telling epoll the supervisor to watch if :
// the client want to say something EPOLLIN
// it got out of the theatre EPOLLRDHUP
// it edge triggered, avoid having the same client with the same message twice, avoid repetitions
// we bind the special ticket to the client so the supervisor know who to supervise
// we send the ticket to epoll, the supervisor, so it can start supervising the client
		// if we can't tell it to epoll, we take out the client from our register and we kick his ass out and block its access

/* ***
 * 7) As parc gatekeeper 
 * 1) let all visitors that are waiting enter the parc
 * 2) check if there's any trouble authenticating the entrance
 * 3) give them a special badge to supervise them
 * 4) add them to the register of visitors 
 * 5) ask the supervisor to have a glance on them 
 * 6) if one don't want to be supervised we kick them ass out
 * *** */

/*	*
 *	Esta función es teniendo en cuenta que tiene que sí o sí terminar
 *	en doble CRLF, si solo mirasemos si está o no en el string, se
 *	podría reemplazar por un simple ::find(CRLF)
 */

// 1 hacer que me sirve una pagina 
// 2 hago que la pagina sea dinamica depende del enpoint o de la url
// 3 servir diferentes typos de archivos y tamanos de resquest

/* ***
 *  777) This is the screen where the camera used by the supervisor
 *  is projected, it's always running to see what the clients are doing
 *  (0 int eventCount = epoll_wait(_epoll_fd, _events, MAX_EVENTS, -1); 0)
 *  1) _epoll_fd is the camera ( epoll using )
 *  2) _events is the list of events detected by the supervisor
 *  3) MAX_EVENTS, the number of events we can cover at a time
 *  4) -1, indefinitely waiting for a client to do something
 *  	no matter what a client does epoll tells us ( TALK, LEAVE ) etc.
 *  5) if (errno == EINTR) continue; if the we were stopped by a signal from 
 *  	the system we restart without stopping the server
 *  6) for (int i = 0; i < eventCount; i++), we covered all the events detected 
 *  7) we retrieved the badge of the concerned client, int clientfd = _events[i].data.fd;
 *  8) if (_events[i].events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)), we check if 
 *  	there's a prob with the client :
 *  	EPOLLERR, an error is detected
 *  	EPOLLHUP, client left
 *  	EPOLLRDHUP, client close its connection
 *  9) close(clientfd); we close the door to this client and removed it 
 *  	from the list, _clientList.erase(clientfd);
 * *** */
