/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 10:39:33 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/03/05 16:02:06 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

#define BUFFER_SIZE 1024

class WebServer
{
	public:

		WebServer(int port);
		~WebServer(void);
		
		void	run(void);

	private:
	
		int							_serverSocket;
		int							_port;
		std::vector<pollfd>			_fds;
		std::map<int, std::string>	_clients;

		void	setupServer();
		void	acceptClient();
		void	handleClient(int clientFd);
		void	sendResponse(int clientFd);
};

#endif
