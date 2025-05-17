/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSettup.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:15:48 by avolcy            #+#    #+#             */
/*   Updated: 2025/05/17 17:19:37 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerSettup.hpp"

ServerSettup::ServerSettup( void ) {}


void	ServerSettup::setNonBlocking( int socketFd, bool enable )
{
	int flags = fcntl( socketFd, F_GETFL, 0 );
	if ( flags  == -1 )
	{
		_socket.close();
		return (perror( "<SERVER> fcntl failed !"));
	}	
	if ( enable )
	{
		if ( fcntl( socketFd, F_SETFL, flags | O_NONBLOCK ) == -1 )
		{
			_socket.close();
			return ( perror("<SERVER> Error Activating O_NONBLOCK"));
		}
	}
	else
	{
		if ( fcntl( socketFd, F_SETFL, flags & ~O_NONBLOCK) == -1 )
		{
			_socket.close();
			return ( perror("<SERVER> Error deActivating O_NONBLOCK"));
		}
	}
}
ServerSettup~ServerSettup( void );