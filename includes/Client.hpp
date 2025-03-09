/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  Client.hpp                                           :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/03/06 18:27:09 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/03/09 21:50:09 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ConnectSocket.hpp"
# include <string>
# define BUF_SIZE 10240

class	Client
{
	public:
		~Client( void );
		Client( int domain, int type, int protocol );
		Client( Client const &obj );

		void		connect( int ip, int port );
		void		send( std::string message );
		std::string	recive( void );

		Client	& operator = ( Client const &obj );

	private:
		ConnectSocket	_socket;
};

#endif
