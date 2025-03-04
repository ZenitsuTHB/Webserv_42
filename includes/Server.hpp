/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 10:39:33 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/03/04 13:38:10 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"

class Server
{
	public:

		Server(void);
		~Server(void);
		
		void	run(void);

	private:
	
		Socket	*_socket;
		int		_socketReq;

};

#endif
