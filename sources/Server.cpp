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
}

int	Server::accept(void)
{
	BaseSocket	client;
	int			statusFlag;

	std::cout << "<SERVER> Accepting client..." << std::endl;
	client = _socket.accept();
	if (client.getSockFd() == -1)
		return (-1);
	statusFlag = fcntl(client.getSockFd(), F_GETFL) | O_NONBLOCK;
	if (fcntl(client.getSockFd(), F_SETFL, statusFlag) == -1)
		return (client.close(), -1);
	_clientList.push_back(client);
	std::cout << "<SERVER> I just took the size of the list..." << _clientList.size() << std::endl;
	return (_clientList.size());
}

/*	*
 *	Esta función es teniendo en cuenta que tiene que sí o sí terminar
 *	en doble CRLF, si solo mirasemos si está o no en el string, se
 *	podría reemplazar por un simple ::find(CRLF)
 */
bool	endRequest(std::string message)
{
	size_t	len;

	len = message.length();
	if (len <= 3)
		return (false);
	return (message.find("\r\n\r\n", len - 4) == std::string::npos)
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

void	Server::respond(std::string response, int idx) const
{
	int	bytes;

	if (idx < 0 || (size_t)idx >= _clientList.size())
		return ;
	bytes = write(_clientList[idx].getSockFd(), response.c_str(),
			response.length());
	if (bytes == -1)
		return (perror("<Server> Error responding"));
}

void	Server::close(int idx)
{
	if (idx < 0 || (size_t)idx >= _clientList.size() || _clientList.empty())
		return ;
	_clientList[idx].close();
	_clientList.erase(_clientList.begin() + idx);
}

std::string	Server::manage(std::string request) const
{
	std::cout << "Request received:" << std::endl << request << std::endl;
	//return ("This is the Server Respond\r\n\r\n");
	std::string http_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 25\r\n"
        "Connection: close\r\n"
        "\r\n"
        "This is the Server Respond";

    return http_response;
}

int	Server::storeFdsset( int idx )//idx debugging purpose
{
	std::cout << "<SERVER> New client: " << idx << " of : ( " << _clientList.size() 
		  << " )" << std::endl;
	if ( _clientList.empty())
	{
		std::cerr << "<SERVER> The client list seems to be empty" << std::endl;
		return ( 0 );
	}

	struct epoll_event newClient;

	_clientfd  = _clientList.back().getSockFd();
	if( _clientfd == -1 )
	{
		std::cerr << "Invalid clientfd !" << std::endl;
		return ( 0 );
	}

	newClient.events = EPOLLIN | EPOLLET;//epollet keep fds from being process in loop but only when new data arrived
	newClient.data.fd = _clientfd;
	std::cout << "<SERVER> Storing client... fd is : [" << this->_clientfd << "] Filling the epollfd to read..." << std::endl;

	int ready = epoll_ctl(  _epoll_fd, EPOLL_CTL_ADD, _clientfd, &newClient );
	std::cout << "\n<SERVER> This is the epoll result : " << ready << std::endl;
	return ( ready );
}

void	Server::run(void)
{
	int idx = 0;
	int fdReady = 0;

	while (true)
	{
		idx = accept( );
		std::cout << "<SERVER> this is from accept : [ " << idx << " ]\n";
		if ( idx != -1 )
			fdReady = storeFdsset( idx );
		if ( fdReady == -1 )
		{
			perror( "<SERVER> epoll_ctl failed !\n" );
			close( _clientfd );
			continue ;
		}
		int counter = epoll_wait( _epoll_fd, _events, MAX_EVENTS, 100 );
		if ( counter == -1 )
		{
			perror( "<SERVER> Error epoll_wait\n" );
			continue ;
		}
		if ( counter == 0 )
		{
			std::cout << "<SERVER> Time OUT ! No Client is ready yet !\n";
			continue ;
		}

		std::cout << "<SERVER> Looking for reads\n" << std::endl;
		for ( int i = 0; i < counter; i++ )
		{
			int clientfd = _events[i].data.fd;

			if ( !(_events[i].events & EPOLLIN))
				continue;
			
			
			std::cout << "<SERVER> managing: [" << i << "]" << std::endl;
			std::string request = receive( i );
			if ( request.empty() )
			{
				std::cout << "Closing client: " << clientfd << std::endl;
				epoll_ctl( _epoll_fd, EPOLL_CTL_DEL, clientfd, NULL );
				close( clientfd );
				continue ;
			}
			std::string response = manage(request);
			respond(response, i );

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
