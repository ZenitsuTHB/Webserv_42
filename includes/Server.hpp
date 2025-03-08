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
# include <list>
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
		std::string	receive( std::list<BaseSocket>::iterator it ) const;
		void		respond( std::string response, std::list<BaseSocket>::iterator it ) const;
		void		close( std::list<BaseSocket>::iterator it );

		Server	& operator = ( Server const &obj );

	private:
		ListenSocket			_socket;
		BaseSocket				*_clientList;
		std::list<BaseSocket>	_clientList2;
		int						_clientNum;
};

#endif
