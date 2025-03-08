/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  BaseClient.hpp                                       :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/03/06 18:27:09 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/03/06 20:23:43 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASECLIENT_HPP
# define BASECLIENT_HPP

# include "ConnectSocket.hpp"
# include <string>
# define BUF_SIZE 10240

class	BaseClient
{
	public:
		~BaseClient( void );
		BaseClient( int domain, int type, int protocol );
		BaseClient( BaseClient const &obj );

		void		connect( int ip, int port );
		void		send( std::string message );
		std::string	recive( void );

		BaseClient	& operator = ( BaseClient const &obj );

	private:
		ConnectSocket	_socket;
};

#endif
