/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  ListenSocket.hpp                                     :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/03/07 21:01:53 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/03/07 21:49:16 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENSOCKET_HPP
# define LISTENSOCKET_HPP

# include "BaseSocket.hpp"

class	ListenSocket: public BaseSocket
{
	public:
		~ListenSocket( void );
		ListenSocket( int domain, int type, int protocol );
		ListenSocket( int domain, int type, int protocol, int backlog);
		ListenSocket( ListenSocket const &obj );

		void		bind( int ip, int port );
		void		listen( int backlog );
		BaseSocket	accept( void ) const;

		ListenSocket	& operator = ( ListenSocket const &obj );

	private:
		int	_backlog;
};

#endif
