/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  BaseServer.cpp                                       :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/03/07 21:48:44 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/03/08 03:16:25 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "../includes/BaseServer.hpp"

BaseServer::~BaseServer(void)
{
	for (int i = 0; i < _clientNum; i++)
		_clientList[i].close();
	delete [] _clientList;
}

BaseServer::BaseServer(int domain, int type, int protocol):
	_socket(domain, type, protocol), _clientList(NULL), _clientNum(0)
{
	int	flags;
	int	fd;
	
	fd = _socket.getSockFd();
	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
	{
		perror("<ListenSocket> fcntl failed");
		return ;
	}
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) == -1)
		perror("<ListenSocket> Error to add O_NONBLOCK");
	return ;
}

BaseServer::BaseServer(BaseServer const &obj):
	_socket(obj._socket), _clientList(NULL), _clientNum(0)
{
	*this = obj;
}

void	BaseServer::start(int ip, int port, int backlog)
{
	_socket.bind(ip, port);
	_socket.listen(backlog);
}

int	BaseServer::accept(void)
{
	BaseSocket	client;
	BaseSocket	*newList;

	client = _socket.accept();
	if (client.getSockFd() == -1)
		return (-1);
	newList = new BaseSocket[_clientNum + 1];
	if (!newList)
		return (perror("Couldn't mallocate client array"), -1);
	for (int i = 0; i < _clientNum; ++i)
		newList[i] = _clientList[i];
	newList[_clientNum] = client;
	delete [] _clientList;
	_clientList = newList;
	++_clientNum;
	return (_clientNum - 1);
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

std::string	BaseServer::recive(int idx) const
{
	std::string	request;
	char		buffer[BUFF_SIZE + 1];
	int			bytes;

	if (idx < 0 || idx >= _clientNum)
		return ("Invalid Index");
	while (true)
	{
		bytes = read(_clientList[idx].getSockFd(), buffer, BUFF_SIZE);
		if (bytes == -1)
			return (perror("Error reading"), std::string());
		buffer[bytes] = '\0';
		request.append(buffer);
		if (endRequest(request))
			break ;
	}
	return (request);
}

void	BaseServer::respond(std::string response, int idx) const
{
	int	bytes;

	if (idx < 0 || idx >= _clientNum)
		return ;
	bytes = write(_clientList[idx].getSockFd(), response.c_str(),
			response.length());
	if (bytes == -1)
		return (perror("Error responding"));
}

void	BaseServer::close(int idx)
{
	BaseSocket	*newList;

	if (idx < 0 || idx >= _clientNum)
		return ;
	_clientList[idx].close();
	--_clientNum;
	if (!_clientNum)
	{
		delete [] _clientList;
		_clientList = NULL;
		return ;
	}
	newList = new BaseSocket[_clientNum];
	if (!newList)
		return (perror("Couldn't mallocate client array"));
	for (int i = 0, j = 0; i <= _clientNum; ++i)
	{
		if (j != idx)
		{
			newList[j] = _clientList[j];
			++j;
		}
	}
	delete [] _clientList;
	_clientList = newList;
}

BaseServer	&BaseServer:: operator = (BaseServer const &obj)
{
	if (this != &obj)
	{
		_socket = obj._socket;
		_clientNum = obj._clientNum;
	}
	return (*this);
}

/*
#include <iostream>
int	main(void)
{
	BaseServer	server(AF_INET, SOCK_STREAM, 0);
	std::string	request;
	int			i;

	server.start(INADDR_ANY, 8080, SOMAXCONN);
	i = server.accept();
	std::cout << "Accepted index: " << i << std::endl;
	request = server.recive(i);
	std::cout << "Request:" << std::endl << request << std::endl;
	server.respond("Hola soy el Servidor", i);
	std::cout << "Responded" << std::endl;
	server.close(i);
	std::cout << "Closed" << std::endl;
}*/
