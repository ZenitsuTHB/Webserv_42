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
#include <sys/select.h>
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
	printf("Cerrando a: [%d]: %d\n", idx, _clientList[idx].getSockFd());
	_clientList[idx].close();
	_clientList.erase(_clientList.begin() + idx);
}

std::string	Server::manage(std::string request) const
{
	std::cout << "Request received:" << std::endl << request << std::endl;
	return ("This is the Server Respond\r\n\r\n");
}

void	Server::run(void)
{
	fd_set			set;
	int				idx;
	int				maxFd;
	struct timeval	time;

	while (true)
	{
		std::cout << "Accepting client..." << std::endl;
		idx = accept();
		if (idx != -1)
			std::cout << "New client: " << idx << "(" << _clientList.size()
				<< ")" << std::endl;
		sleep(1);
		/*	*
		 *	PONER LOS FDs EN EL SET A VIGILAR CON SELECT
		 */
		std::cout << "Selecting fd to read..." << std::endl;
		FD_ZERO(&set);
		maxFd = 0;
		for (int i = 0; (size_t)i < _clientList.size(); i++)
		{
			int	clientFd;

			clientFd = _clientList[i].getSockFd();
			if (clientFd > maxFd)
			{
				maxFd = clientFd;
			}
			FD_SET(clientFd, &set);
			std::cout << "Setting [" << i << "]: " << clientFd << std::endl;
		}
		time.tv_usec = 5000;
		time.tv_sec = 0;
		maxFd = select(maxFd + 1, &set, NULL, NULL, &time);//EL PUTO +1
		if (maxFd == -1)
			perror("Selected fallo");
		/*	*
		 *	CONTESTAR TODOS LOS FDs QUE ESTAN DISPONIBLES A LEER
		 */
		std::cout << "Selected: " << maxFd << " of " << _clientList.size() << std::endl;
		sleep(4);
		if (!maxFd)
			continue ;

		std::cout << "Looking for reads" << std::endl;
		for (int i = 0, j = 0; (size_t)i < _clientList.size() && j < maxFd; ++i)
		{
			sleep(1);
			std::string	request;
			std::string	response;
			if (!FD_ISSET(_clientList[i].getSockFd(), &set))
				continue ;
			std::cout << "Server managing: [" << i << "]" << std::endl;
			request = receive(i);
			response = manage(request);
			respond(response, i);
			close(i);
			--i;
			j++;
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
	/*
	std::string	request;
	int			i;

	while (true)
	{
		i = server.accept();
		if (i == -1)
			continue ;
		std::cout << "Accepted index: " << i << std::endl;
		request = server.receive(i);
		std::cout << "Request:" << std::endl << request << std::endl;
		server.respond("Hola soy el Servidor", i);
		std::cout << "Responded" << std::endl;
		server.close(i);
		std::cout << "Closed" << std::endl;
		break ;
	}*/
}
