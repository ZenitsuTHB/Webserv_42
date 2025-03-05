/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:02:03 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/03/05 19:27:14 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Listen.hpp"

Listen::Listen()
{
	_backlog = 10;
}

Listen::Listen(int backlog): _backlog(backlog)
{
}

Listen::~Listen()
{
}

int	Listen::getBacklog() const
{
	return _backlog;
}

void	Listen::listen(BaseSocket const &socket)
{
	int	socketFd = socket.getSockFd();

	if (::listen(socketFd, _backlog) < 0)
	{
		perror("Error in listen <Listen>");
		exit(1);
	}
	
	int	flags = fcntl(socketFd, F_GETFL, 0);
	if (flags == -1)
		perror("Error to obtain the flags <Listen>");

	flags |= O_NONBLOCK;
	
	if (fcntl(socketFd, F_SETFL, flags) == -1)
		perror("Error to add O_NONBLOCK <Listen>");
}

void	Listen::accept(BaseSocket const &socket)
{
	int	socketFd = socket.getSockFd();
	struct sockaddr_in	clientAddr;
	socklen_t			clientLen = sizeof(clientAddr);
	
	int newSockFd = ::accept(socketFd, (struct sockaddr*)&clientAddr, &clientLen);
	if (newSockFd < 0)
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			std::cout << "There are no conexions, waiting..." << std::endl;
		else
			perror("Error in accept <Listen>");
	else
	{
		std::cout << "Conexion accepted!" << std::endl;
		socket.close();
		socket.setSockFd(newSockFd);
	}
}
