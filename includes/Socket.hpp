/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 10:58:19 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/03/04 13:30:57 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

class Socket
{
	public:

		Socket(int domain, int type, int protocol);
		~Socket();

		void	bindSocket(int port, int ip);

		int		getSocketFd() const;

	private:

		int					_socketFd;
		int					_domain;
		int					_type;
		int					_protocol;
		int					_port;
		int					_ipAddress;
		struct sockaddr_in	_addr;

};

#endif
