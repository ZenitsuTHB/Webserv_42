/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  Client.cpp                                           :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/03/06 18:27:07 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/03/09 21:49:58 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "../includes/Client.hpp"

Client::~Client(void)
{
	return ;
}

Client::Client(int domain, int type, int protocol):
	_socket(domain, type, protocol)
{
	return ;
}

Client::Client(Client const &obj):
	_socket(obj._socket)
{
	*this = obj;
}

void	Client::connect(int ip, int port)
{
	_socket.connect(ip, port);
}

void	Client::send(std::string message)
{
	if (write(_socket.getSockFd(), message.c_str(), message.length()) == -1)
		return (perror("Message not havle to send"));
}

std::string	Client::recive( void )
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

Client	&Client:: operator = (Client const &obj)
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
	Client	client(AF_INET, SOCK_STREAM, 0);
	std::string	response;

	client.connect(INADDR_ANY, 8080);
	client.send("HOla soy el cliente en CP");
	response = client.recive();
	std::cout << "Response from server:" << std::endl << response << std::endl;
}*/
