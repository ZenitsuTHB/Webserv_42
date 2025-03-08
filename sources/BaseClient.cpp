/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  BaseClient.cpp                                       :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/03/06 18:27:07 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/03/07 21:25:15 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "../includes/BaseClient.hpp"

BaseClient::~BaseClient(void)
{
	return ;
}

BaseClient::BaseClient(int domain, int type, int protocol):
	_socket(domain, type, protocol)
{
	return ;
}

BaseClient::BaseClient(BaseClient const &obj):
	_socket(obj._socket)
{
	*this = obj;
}

void	BaseClient::connect(int ip, int port)
{
	_socket.connect(ip, port);
}

void	BaseClient::send(std::string message)
{
	if (write(_socket.getSockFd(), message.c_str(), message.length()) == -1)
		return (perror("Message not havle to send"));
}

std::string	BaseClient::recive( void )
{
	std::string	message;
	char		buffer[BUF_SIZE];
	int			bytes;

	bytes = read(_socket.getSockFd(), buffer, sizeof(buffer));
	if (bytes == -1)
		return (perror("Not sé leer"), message);
	buffer[bytes] = '\0';
	message = buffer;
	return (message);
}

BaseClient	&BaseClient:: operator = (BaseClient const &obj)
{
	if (this != &obj)
	{
		_socket = obj._socket;
	}
	return (*this);
}

/*
#include <iostream>

int	main(void)
{
	BaseClient	client(AF_INET, SOCK_STREAM, 0);
	std::string	response;

	client.connect(INADDR_ANY, 8080);
	client.send("HOla soy el cliente en CP");
	response = client.recive();
	std::cout << "Response from server:" << std::endl << response << std::endl;
}*/
