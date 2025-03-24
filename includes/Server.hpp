/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 0025/03/07 21:48:42 by velazqu           #+#    #+#             */
/*   Updated: 2025/03/24 16:30:16 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <vector>
# include "ListenSocket.hpp"
# include <sys/epoll.h>

# define BUFF_SIZE 100
# define MAX_EVENTS 5

class	Server
{
	public:
		~Server( void );
		Server( int domain, int type, int protocol );
		Server( Server const &obj );

		void		start( int ip, int port, int backlog );
		int			accept( void );
		std::string	receive( int idx ) const;
		std::string	manage( std::string request ) const;
		void		respond( std::string response, int idx ) const;
		void		close( int idx );
		int			storeFdsset( int );
		void		run( void );

		Server	& operator = ( Server const &obj );

	private:
		int				_clientfd;
		ListenSocket			_socket;
		std::vector<BaseSocket>		_clientList;
		int				_epoll_fd;
		struct	epoll_event		_events[MAX_EVENTS];
};

#endif
