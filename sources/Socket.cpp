/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 11:09:07 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/03/05 13:56:57 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Socket.hpp"

Socket::Socket(int domain, int type, int protocol): _domain(domain), _type(type), _protocol(protocol)
{
	if ((_socketFd = socket(_domain, _type, _protocol)) == -1)
		perror("Error in socket");
	int	opt = 1;
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		perror("Error in setsockopt");
}
	
Socket::~Socket()
{
	close(_socketFd);
}

void	Socket::bindSocket(int port, int ip)
{
	_port = port;
	_ipAddress = ip;

	bzero(&_addr, sizeof(_addr));
	_addr.sin_family = _domain;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(_socketFd, (struct sockaddr*)&_addr, sizeof(_addr)) == -1)
		perror("Error in bind");
}

int	Socket::getSocketFd() const
{
	return _socketFd;
}
