/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 0025/03/07 21:01:53 by velazqu           #+#    #+#             */
/*   Updated: 2025/04/04 18:27:50 by avolcy           ###   ########.fr       */
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
		ListenSocket( int domain, int type, int protocol, int backlog );
		ListenSocket( ListenSocket const &obj );

		void		bind( int ip, int port );
		void		listen( int backlog );
		BaseSocket	accept( void ) const;
		void		setNonBlocking( int, bool );
		

		ListenSocket	& operator = ( ListenSocket const &obj );

	private:
		int	_backlog;
};

#endif
