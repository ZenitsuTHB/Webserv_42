/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  Server.hpp                                           :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/03/07 21:48:42 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/03/08 14:16:44 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASESERVER_HPP
# define BASESERVER_HPP

# include <string>
# include "ListenSocket.hpp"

# define BUFF_SIZE 100

class	Server
{
	public:
		~Server( void );
		Server( int domain, int type, int protocol );
		Server( Server const &obj );

		void		start( int ip, int port, int backlog );
		int			accept( void );
		std::string	receive( int idx ) const;
		void		respond( std::string response, int idx ) const;
		void		close( int idx );

		Server	& operator = ( Server const &obj );

	private:
		ListenSocket	_socket;
		BaseSocket		*_clientList;
		int				_clientNum;
};

#endif
