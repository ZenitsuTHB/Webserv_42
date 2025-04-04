/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSocket.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 0025/03/05 18:26:18 by velazqu           #+#    #+#             */
/*   Updated: 2025/04/04 18:21:13 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/BaseSocket.hpp"
#include <unistd.h>
#include <iostream>
#include <stdio.h>

BaseSocket::~BaseSocket(void)
{
	return ;
}

BaseSocket::BaseSocket(void): _type(0),  _sockFd(-1), _domain(0), _protocol(0),
	_addrLen(sizeof(_address))
{
	_address.sin_family = 0;
	_address.sin_port = 0;
	_address.sin_addr.s_addr = 0;
	return ;
}

BaseSocket::BaseSocket(int fd): _type(0), _sockFd(fd), _domain(0), _protocol(0),
	_addrLen(sizeof(_address))
{
	_address.sin_family = 0;
	_address.sin_port = 0;
	_address.sin_addr.s_addr = 0;
}

BaseSocket::BaseSocket(int domain, int type, int protocol):
	_sockFd(-1), _addrLen(sizeof(_address))
{
	_address.sin_family = domain;
	_address.sin_port = 0;
	_address.sin_addr.s_addr = 0;
	create(domain, type, protocol);
	return ;
}

BaseSocket::BaseSocket(BaseSocket const &obj): _sockFd(-1)
{
	*this = obj;
}

void	BaseSocket::create(int domain, int type, int protocol)
{
	if (_sockFd != -1)
		return ;
	_sockFd = socket(domain, type, protocol);
	if (_sockFd == -1)
		return (perror("<BaseSocket> Error sucking"));
	_protocol = protocol;
	_domain = domain;
	_type = type;
	_address.sin_family = domain;
}

void	BaseSocket::bind(struct sockaddr_in address)
{
	_address = address;
	if (::bind(_sockFd, (const struct sockaddr *)&_address, sizeof(_address)) == -1)
		return (perror("<BaseSocket> Error binding"), close());
}

void	BaseSocket::bind(int ip, int port)
{
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = htonl(ip);
	if (::bind(_sockFd, (const struct sockaddr *)&_address, sizeof(_address)) == -1)
		return (perror("<BaseSocket> Error binding"), close());
}

void	BaseSocket::close(void)
{
	if (_sockFd == -1)
		return ;
	printf("Closing: %d\n", _sockFd);
	::close(_sockFd);
	_sockFd = -1;
}

void	BaseSocket::setSockFd(int fd)
{
	if (_sockFd != -1)
		::close(_sockFd);
	_sockFd = fd;
}

void	BaseSocket::setAddress( struct sockaddr_in address, socklen_t len)
{
	if (len > _addrLen)
		std::cerr << "<BaseSocket> Setted address lenght too long" << std::endl;
	_addrLen = len;
	_address = address;
}

BaseSocket	&BaseSocket:: operator = (BaseSocket const &obj)
{
	if (this != &obj || _sockFd != obj._sockFd)
	{
		//close();
		_sockFd = obj._sockFd;
		_domain = obj._domain;
		_type = obj._type;
		_protocol = obj._protocol;
		_address = obj._address;
		_addrLen = obj._addrLen;
	}
	return (*this);
}
