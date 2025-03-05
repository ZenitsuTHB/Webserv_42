/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  BaseSocket.hpp                                       :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/03/05 18:26:19 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/03/05 19:53:22 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASESOCKET_HPP
# define BASESOCKET_HPP

# include <sys/socket.h>
# include <netinet/in.h>

class	BaseSocket
{
	public:
		~BaseSocket( void );
		BaseSocket( void );
		BaseSocket( int fd );
		BaseSocket( int domain, int type, int protocol );
		BaseSocket( BaseSocket const &obj );

		void	create( int domain, int type, int protocol );
		void	bind( struct sockaddr_in address );
		void	bind( int ip, int port );
		void	close( void );

		void	setSockFd( int fd );
		void	setAddress( struct sockaddr_in address, socklen_t len);

		int	getSockFd( void ) const { return (_sockFd); };
		int	getPort( void ) const { return (htons(_address.sin_port)); }
		int	getIp( void ) const { return (htonl(_address.sin_addr.s_addr)); }

		BaseSocket	& operator = ( BaseSocket const &obj );

	private:
		int					_sockFd;
		int					_domain;
		int					_type;
		int					_protocol;
		struct sockaddr_in	_address;
		socklen_t			_addrLen;
};

#endif
