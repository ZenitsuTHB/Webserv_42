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
//#include <sys/select.h>
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

	client = _socket.accept();
	if (client.getSockFd() == -1)
		return (-1);
	statusFlag = fcntl(client.getSockFd(), F_GETFL) | O_NONBLOCK;
	if (fcntl(client.getSockFd(), F_SETFL, statusFlag) == -1)
		return (client.close(), -1);
	_clientList.push_back(client);
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
	if (message.find("\r\n\r\n", len - 4) == std::string::npos)
		return (false);
	return (true);
}

std::string	Server::receive(int idx) const
{
	std::string	request;
	char		buffer[BUFF_SIZE + 1];
	int			bytes;

	if (idx < 0 || (size_t)idx >= _clientList.size())
		return ("<Server> Invalid Index");
	while (true)
	{
		bytes = read(_clientList[idx].getSockFd(), buffer, BUFF_SIZE);
		if (bytes == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break ;
			return (perror("<Server> Error reading"), std::string());
		}
		if (bytes == 0)
			break ;
		buffer[bytes] = '\0';
		request.append(buffer);
		//if (endRequest(request))
		//	break ;
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
	return ("This is the Server Respond\r\n\r\n");
}

int	Server::storeFdsset( )
{
	int idx;

	std::cout << "Accepting client..." << std::endl;
	idx = accept();
	if (idx != -1)
	{
		std::cout << "New client: " << idx << "(" << _clientList.size() 
			  << ")" << std::endl;
		sleep(1);
		std::cout << "Filling the pollfd to read..." << std::endl;
		struct pollfd	pfd;
		pfd.fd = _clientList.back().getSockFd();
		pfd.events = POLLIN;//surveiller la lecture
		_pollfds.push_back( pfd );
	}
	int ready = poll( _pollfds.data(), _pollfds.size(), 2000);
	return ( ready );
}

void	Server::run(void)
{

	while (true)
	{
		int maxFd = storeFdsset( );
		if (maxFd == -1)
		{
			perror( "poll failed !" );
			continue ;
		}
		sleep(4);
		if (maxFd == 0)
		{
			perror( "Time OUT  ! No Client is ready yet !" );
			continue ;
		}

		std::cout << "Looking for reads\n" << std::endl;
		for (size_t i = 0; i < _pollfds.size(); )
		{
			if (!(_pollfds[i].revents & POLLIN))
			{
				++i;
				continue;
			}

			std::cout << "Server managing: [" << i << "]" << std::endl;
			std::string request = receive(i);
			std::string response = manage(request);
			respond(response, i);

			close(i);
			_pollfds.erase(_pollfds.begin() + i); // Supprimer l'entrée pollfd
	//	size_t i = 0;
	//	int checked_clients =0;
	//	int clientsTochecke = _pollfds.size();
	//	while ( i < _clientList.size() && checked_clients < clientsTochecke ) 
	//	{
	//		std::cout << "Selected: " << i << " of " << _clientList.size() << std::endl;
	//		sleep(4);//to delete
	//		if ( !( _pollfds[i].revents & POLLIN ) )
	//		{
	//			i++;
	//			continue ;
	//		}
	//		std::string request = receive( i );
	//		std::string response = manage( request );
	//		respond( response, i );
	//		close( i );
	//		--i;
	//		checked_clients++;
		}


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
