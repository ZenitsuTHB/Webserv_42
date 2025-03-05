/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:10:14 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/03/05 18:00:34 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServer.hpp"

WebServer::WebServer(int port): _port(port)
{
}
		
WebServer::~WebServer(void)
{
}
		
void	WebServer::run(void)
{
	setupServer();

	while (true)
	{
		int	poll_count = poll(&_fds[0], _fds.size(), -1);
		if (poll_count < 0)
		{
			std::cerr << "Poll error: " << strerror(errno) << std::endl;
			break ;
		}

		for (size_t i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _serverSocket)
					acceptClient();
				else
					handleClient(_fds[i].fd);
			}
		}
	}
}

void	WebServer::setupServer()
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1)
	{
		std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	int opt = 1;
	setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(_port);

	if (bind(_serverSocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cerr << "Bind failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(_serverSocket, 10) < 0)
	{
		std::cerr << "Listen failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	
	pollfd server_fd;
	server_fd.fd = _serverSocket;
	server_fd.events = POLLIN;
	_fds.push_back(server_fd);

	std::cout << "Server listening on port " << _port << std::endl;
}

void	WebServer::acceptClient()
{
	sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	
	int client_socket = accept(_serverSocket, (struct sockaddr*)&client_addr, &client_len);
	
	if (client_socket < 0)
	{
		std::cerr << "Accept failed: " << strerror(errno) << std::endl;
		return;
	}
	
	open
	fcntl(client_socket, F_SETFL, O_NONBLOCK);
	pollfd client_fd;
	client_fd.fd = client_socket;
	client_fd.events = POLLIN;
	_fds.push_back(client_fd);
	_clients[client_socket] = "";
	
	std::cout << "New connection accepted." << std::endl;
}

void	WebServer::handleClient(int clientFd)
{
	char buffer[BUFFER_SIZE];
	
	memset(buffer, 0, BUFFER_SIZE);
	
	int bytes_read = recv(clientFd, buffer, BUFFER_SIZE - 1, 0);

	if (bytes_read <= 0)
	{
		std::cout << "Client disconnected." << std::endl;
		close(clientFd);
		_clients.erase(clientFd);
		return;
	}

	_clients[clientFd] += std::string(buffer, bytes_read);

	if (_clients[clientFd].find("\r\n\r\n") != std::string::npos)
	{
		sendResponse(clientFd);
		_clients[clientFd] = "";
	}
}
	
void	WebServer::sendResponse(int clientFd)
{
	std::string	response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 13\r\n\r\nHello, World!";
	send(clientFd, response.c_str(), response.size(), 0);
	close(clientFd);
}
