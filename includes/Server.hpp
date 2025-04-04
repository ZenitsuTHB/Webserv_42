/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 0025/03/07 21:48:42 by velazqu           #+#    #+#             */
/*   Updated: 2025/04/04 18:29:52 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <cstring>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <vector>
# include "ListenSocket.hpp"
# include <sys/epoll.h>
# include <map>
# include <cstddef>
# include <cstdlib>

static const size_t BUFF_SIZE = 42;
//# define BUFF_SIZE 42
# define MAX_EVENTS 5
# define PORT 8080
# define BACKLOG 10

class	Server : public ListenSocket
{
	public:
		~Server( void );
		Server( int domain, int type, int protocol );
		Server( Server const &obj );

		void				shutDownServer();
		void		run( void );
		std::string	receive( int idx ) const;
		std::string	manage( std::string request ) const;
		void		start( int ip, int port, int backlog );
		void		respond( std::string response, int idx ) const;

		Server& 	operator = ( Server const &obj );

	private:
		ListenSocket			_socket;
		int				_epoll_fd;
		bool				_running;
		int				_clientfd;
		std::map< int, BaseSocket >	_clientsMap;
		//std::map< int, time_t > 		_timeoutMap;
		std::vector< char >			_buffer;
		struct	epoll_event		_events[MAX_EVENTS];

		void				closeClient( int clientFd );
		void				acceptNewConnection( void ); 
		void				handleClientEvent( int clientFd, uint32_t events ); 

};
#endif
