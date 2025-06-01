/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:55:48 by avolcy            #+#    #+#             */
/*   Updated: 2025/06/01 20:03:27 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <poll.h>
#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <sys/errno.h>
#include "../../includes/Server.hpp"
#include "../../includes/HttpResponse.hpp"

//siege -g 127.0.0.1:8080 
//request from term "echo -e GET / HTTP/1.1\r\nHost: localhost\r\n\r\n | nc localhost 8080"
#define LOG( msg ) std::cout << "[SERVER]\n" << msg << std::endl

static int requestCount = 0;

Server::Server(in_addr_t ip, in_port_t port, int backlog , int shared_epoll): 
	_socket(AF_INET, SOCK_STREAM, 0), _buffer(BUFF_SIZE)
{
	int     fd;
	int     opt;

	fd = _socket.getSockFd();
	opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		_socket.close();
		throw std::runtime_error("<Server> setsockopt(SO_REUSEADDR) failed");
	}
	setNonBlocking( fd, true );

	_clientfd = -1;
	//_epoll_fd = epoll_create1( 0 );
	if ( _epoll_fd == -1 )
		perror("<SERVER> Failed to create epoll instance");
	start(ip, port, backlog);
	return ;
}

Server::~Server(void) { return ;}

void	Server::setNonBlocking( int socketFd, bool enable )
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

Server::Server( Server const &obj ) : BaseSocket(obj), _socket(obj._socket),
	_epoll_fd(obj._epoll_fd), _running(obj._running), _clientfd(obj._clientfd),
	_clientsMap(obj._clientsMap), _buffer(obj._buffer) { return ;}

Server&	Server::operator = ( Server const &obj )
{
	if ( this != &obj )
	{       
		BaseSocket::operator=(obj);
		_socket = obj._socket;
		_epoll_fd = obj._epoll_fd;
		_running = obj._running;
		_clientfd = obj._clientfd;
		_clientsMap = obj._clientsMap;
		_buffer = obj._buffer;
	}
	return ( *this );
}

void	Server::shutDownServer(void)
{
	std::map< int, BaseSocket >::iterator it;
	_running = false;
	for ( it = _clientsMap.begin(); it != _clientsMap.end(); ++it )
		it->second.close();
	//::close( _epoll_fd );
	_socket.close();
	std::cout << "<SERVER> Shutdown complete." << std::endl;
}

void    Server::closeClient( int fd )
{
	std::map< int, BaseSocket >::iterator it = _clientsMap.find( fd ); 
	if ( it == _clientsMap.end() ) return;

	if ( epoll_ctl( _epoll_fd, EPOLL_CTL_DEL, fd, NULL ) == -1)
	{
		if ( errno != EBADF && errno != ENOENT)
			perror("<SERVER> ERR removing client from epoll\nPerror ");
	}
	_clientsMap.erase( it );
	//::close( fd );
	//or it->second.close();
}

void	Server::disconnectingClient( int fdClient, uint32_t events )
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
				return( perror( "<SERVER> Error accepting client" ));
		}

		_clientsMap[clientSocket.getSockFd()] = clientSocket;
		setNonBlocking( clientSocket.getSockFd(), true );
		//_socket.getSockFd() fd du Server

		struct epoll_event event;

		event.events = EPOLLIN | EPOLLRDHUP | EPOLLET | EPOLLHUP;
		event.data.fd = clientSocket.getSockFd();

		if ( epoll_ctl( _epoll_fd, EPOLL_CTL_ADD, clientSocket.getSockFd(), &event ) == -1 )
		{
			perror( "<SERVER> Failed to add client to epoll" );
			closeClient( clientSocket.getSockFd() );
		}
	}
	return ;
}

// EPOLLHUP the connection get entirely cut off
// EPOLLRDHUP the client shut its part using close or 
// shutdown(fd , SHUT_WR) and it can still receive data

void Server::prepareStaticResponse() {
	std::ifstream file("./html/index.html");
	if (!file) {
		std::cerr << "<SERVER> Failed to load index.html at startup.\n";
		_responseReady = false;
		return;
	}

	std::string body( (std::istreambuf_iterator< char >( file )),  std::istreambuf_iterator< char >());
	std::ostringstream response;
	response << "HTTP/1.1 200 OK\r\n"
		<< "Content-Type: text/html\r\n"
		<< "Content-Length: " << body.size() << "\r\n\r\n"
		<< body;

	_cachedResponse = response.str();
	_responseReady = true;
	LOG("index.html preloaded successfully (" << body.size() << " bytes)");
}

void Server::requestResponse(int fd)
{
	if ( !_responseReady )
	{
		std::string notFound = "HTTP/1.1 404 Not Found\r\nContent-Length:13\r\n\r\n404 Not Found";
		write(fd, notFound.c_str(), notFound.size());
		//closeClient(fd);
		return;
	}

	size_t totalSent = 0;
	while (totalSent < _cachedResponse.size()) {
		ssize_t sent = write(fd, _cachedResponse.c_str() + totalSent, _cachedResponse.size() - totalSent);
		if (sent == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			LOG("Write error to client " << fd << ": " << strerror(errno));
			//closeClient(fd);
			return;
		}
		totalSent += sent;
	}
}

std::string	Server::readRequest( int ftFlClient )
{
	std::vector<char> dynamBuff(1024);
	std::string &buffer = _recvBuffers[ ftFlClient ];
	ssize_t bytesRead = 0;

	while( true )
	{
		bytesRead = read( ftFlClient  , dynamBuff.data(), dynamBuff.size() );
		if ( bytesRead > 0 )
			buffer.append( dynamBuff.data(), bytesRead );
		else if ( bytesRead == 0 )
		{
			LOG("Client [" << ftFlClient  << "] closed connection.");
			closeClient( ftFlClient );
			break ;
		}
		else
		{
			if ( errno == EAGAIN || errno == EWOULDBLOCK )
			{
				LOG("No more data to read");
				break ; 
			}
			else
			{
				LOG("Read error on Client fd [" << ftFlClient << "] : " << strerror(errno));
				closeClient( ftFlClient );
				break;
			}

		}
	}
	return ( buffer );
}

void	Server::handleClientEvent( int fdClient, uint32_t events )
{
	if ( events & ( EPOLLRDHUP | EPOLLHUP | EPOLLERR ))
		disconnectingClient( fdClient, events );
	else if ( events & EPOLLIN )
	{
		std::string	reqStr = readRequest( fdClient );
		//	maxiiii
		/*
		   try
		   {
		   HttpRequest		request(reqStr);
		   HttpResponse	response(request);

		   _cachedResponse = response.generate();
		   _responseReady = true;
		   requestCount++;
		   LOG("Request total count: " << requestCount);
		   }
		   catch (std::exception const &ex)
		   {
		   _responseReady = true;
		   _cachedResponse = "HTTP/1.1 404 Not Found\r\nContent-Length:13\r\n\r\n404 Not Found";
		   LOG("Requested failed");
		   }
		   prepareStaticResponse();
		   bool	shouldClose = false;
		   LOG("=========[ HTTP REQUEST ]=========");
		   LOG(reqStr);
		   LOG("=========[    END       ]=========");
		   std::cout << "Response:" << std::endl << _cachedResponse;
		   requestResponse( fdClient );
		   _recvBuffers[ fdClient ].clear();
		   if ( shouldClose )
		   {
		   LOG("Client requested connection close.");
		   closeClient(fdClient);
		   return;
		   }
		   std::cout << "Termina mi parte" << std::endl;
		   */
		//	archly
		if (!reqStr.empty())
		{

			std::string&	buffer = _recvBuffers[ fdClient ];
			size_t	headerEnd = buffer.find(END);

			while( headerEnd != std::string::npos )
			{
				std::string fullRequest = buffer.substr( 0, headerEnd + 4 );//4 for END 
				buffer.erase( 0, headerEnd + 4);
				bool shouldClose = false;
				if (fullRequest.find("Connection: close") != std::string::npos)
					shouldClose = true;
				LOG("=========[ HTTP REQUEST ]=========");
				LOG(fullRequest);
				LOG("=========[    END       ]=========");
				requestResponse( fdClient );
				if ( shouldClose )
				{
					LOG("Client requested connection close.");
					closeClient(fdClient);
					return;
				}
				headerEnd = buffer.find(END);
			}

			requestCount++;
			LOG("Request total count: " << requestCount);
			//HttpRequest	req = Adri.brainParser( reqStr );
			//HttpResponse	res = Maxim.responseArtBuilder( req, config );

			//std::string confidentialStuff = res.serialize();
			//write( fdClient, confidentialStuff.c_str(), confidentialStuff.size());
		}
		//	adioss hasta aqui termina
	}
}

void	Server::run( void )
{
	//std::cout << "This is SErver" << std::endl;
	//return ;
	while ( _running )
	{
		int eventCounter = epoll_wait( _epoll_fd, _events, MAX_EVENTS, -1);
		if ( eventCounter == -1 )
		{
			if( errno == EINTR ) return ; //continue;
			throw std::runtime_error( "<SERVER> epoll_wait failed " + std::string(strerror( errno )));
		}

		for ( int i = 0; i < eventCounter; i++)
		{
			int clientFd = _events[i].data.fd;//new client
			if ( clientFd == _socket.getSockFd() )
				acceptNewConnection();
			else
				handleClientEvent( clientFd, _events[i].events );//manage connected client
		}

	}
}

void    Server::start( in_addr_t ip, in_port_t port, int backlog )
{
	_socket.bind( ip, port );
	_socket.listen( backlog );
	setNonBlocking( _socket.getSockFd() , true );

	struct  epoll_event event;
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = _socket.getSockFd();
	if( epoll_ctl( _epoll_fd, EPOLL_CTL_ADD, _socket.getSockFd(), &event) == -1 )
		throw std::runtime_error( "<SERVER> Failed to add socket to epoll: " +
				std::string(strerror(errno)));
	_running = true;
	prepareStaticResponse();
	std::cout << "<SERVER> started on ip : [ " << ip << " ] and port : [ " << port << " ]\n";
}
