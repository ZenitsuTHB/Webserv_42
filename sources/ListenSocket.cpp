/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  ListenSocket.cpp                                     :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/03/07 21:01:48 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/03/07 21:49:44 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../includes/ListenSocket.hpp"

ListenSocket::~ListenSocket(void)
{
	return ;
}

ListenSocket::ListenSocket(int domain, int type, int protocol):
	BaseSocket(domain, type, protocol)
{
	return ;
}

ListenSocket::ListenSocket(int domain, int type, int protocol, int backlog):
	BaseSocket(domain, type, protocol), _backlog(-1)
{
	listen(backlog);
	return ;
}

ListenSocket::ListenSocket(ListenSocket const &obj):
	BaseSocket()
{
	*this = obj;
}

void	ListenSocket::bind(int ip, int port)
{
	BaseSocket::bind(ip, port);
}

void	ListenSocket::listen(int backlog)
{
	if (::listen(getSockFd(), backlog) == -1)
		return (perror("<ListenSocket> Lisenin no aprobaoh"));
	_backlog = backlog;
}

BaseSocket	ListenSocket::accept(void) const
{
	BaseSocket			request;
	int					reqFd;
	struct sockaddr_in	reqAddr;
	socklen_t			reqLen;

	reqLen = sizeof(reqAddr);
	reqFd = ::accept(getSockFd(), (struct sockaddr *)&reqAddr, &reqLen);
	if (reqFd == -1)
		return (perror("<ListenSocket> Request failed to accept"), request);
	request.setSockFd(reqFd);
	request.setAddress(reqAddr, sizeof(reqAddr));
	return (request);
}

ListenSocket	&ListenSocket:: operator = (ListenSocket const &obj)
{
	if (this != &obj)
	{
		BaseSocket::operator = (obj);
	}
	return (*this);
}
