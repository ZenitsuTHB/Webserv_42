/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:02:23 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/03/05 19:27:23 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTEN_HPP
#define LISTEN_HPP

#include "BaseSocket.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

class Listen: public BaseSocket
{
	public:

		Listen(void);
		Listen(int numCon);
		~Listen(void);

		void	listen(BaseSocket &socket);
		void	accept(BaseSocket &socket);
		int		getBacklog() const;

	private:

		int		_backlog;

};

#endif
