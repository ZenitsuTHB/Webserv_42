/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  ConnectSocket.cpp                                    :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/03/06 16:42:20 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/03/07 21:25:13 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ConnectSocket.hpp"
#include <stdio.h>

ConnectSocket::~ConnectSocket(void)
{
	return ;
}

ConnectSocket::ConnectSocket(int domain, int type, int protocol):
	BaseSocket(domain, type, protocol), _addCLen(sizeof(_addrCon))
{
	_addrCon.sin_family = domain;
	_addrCon.sin_port = 0;
	_addrCon.sin_addr.s_addr = 0;
	return ;
}

ConnectSocket::ConnectSocket(ConnectSocket const &obj): BaseSocket()
{
	*this = obj;
}

void	ConnectSocket::bind( int ip, int port )
{
	BaseSocket::bind(ip, port);
}

void	ConnectSocket::connect(struct sockaddr_in address, socklen_t len)
{
	if (::connect(getSockFd(), (struct sockaddr *)&address, len) == -1)
		return (perror("<ConnectSocket> No conection made"));
	_addrCon = address;
	_addCLen = len;
}

void	ConnectSocket::connect(int ip, int port)
{
	_addrCon.sin_addr.s_addr = htonl(ip);
	_addrCon.sin_port = htons(port);
	if (::connect(getSockFd(), (struct sockaddr *)&_addrCon, _addCLen) == -1)
		return (perror("<ConnectSocket> No conection made"));
}

ConnectSocket	&ConnectSocket:: operator = (ConnectSocket const &obj)
{
	if (this != &obj)
	{
		BaseSocket:: operator = (obj);
		_addCLen = obj._addCLen;
		_addrCon = obj._addrCon;
	}
	return (*this);
}

/*
#include <iostream>

int	main(void)
{
	ConnectSocket	client(AF_INET, SOCK_STREAM, 0);

	client.connect(INADDR_ANY, 8080);
}*/
