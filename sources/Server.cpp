/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 10:54:28 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/03/04 13:50:55 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server()
{
	_socket = new Socket(AF_INET, SOCK_STREAM, 0);
	_socket->bindSocket(8080, INADDR_ANY);
}

Server::~Server()
{
	close(_socketReq);
	delete _socket;
}

void	Server::run()
{
	std::cout << "Servidor iniciado" << std::endl;

	struct sockaddr_storage	addr_req;
	socklen_t	addr_len = sizeof(struct sockaddr_storage);
	char	buffer[1024];
	int		rd_bytes;

	if (listen(_socket->getSocketFd(), 10) == -1)
		perror("Error in listen");

	for (int i = 0; i < 10; i++)
	{
		bzero(buffer, sizeof(buffer));
		bzero(&addr_req, sizeof(addr_req));

		_socketReq = accept(_socket->getSocketFd(), (struct sockaddr*)&addr_req, &addr_len);
		if (_socketReq == -1)
			perror("Error to accept");
		rd_bytes = read(_socketReq, buffer, sizeof(buffer));
		if (rd_bytes == -1)
			return ;
		buffer[rd_bytes] = '\0';
		write(_socketReq, "Hola", 4);
	}
}
