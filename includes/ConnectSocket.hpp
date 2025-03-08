/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  ConnectSocket.hpp                                    :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/03/06 16:42:23 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/03/07 19:41:10 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTSOCKET_HPP
# define CONNECTSOCKET_HPP

# include "../includes/BaseSocket.hpp"

class	ConnectSocket: public BaseSocket
{
	public:
		~ConnectSocket( void );
		ConnectSocket( int domain, int type, int protocol );
		ConnectSocket( ConnectSocket const &obj );

		void	bind( int ip, int port );
		void	connect( struct sockaddr_in address, socklen_t len );
		void	connect( int ip, int port );

		ConnectSocket	& operator = ( ConnectSocket const &obj );

	private:
		struct sockaddr_in	_addrCon;
		socklen_t			_addCLen;
};

#endif
