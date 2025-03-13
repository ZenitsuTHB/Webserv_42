/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epoll.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:04:13 by avolcy            #+#    #+#             */
/*   Updated: 2025/03/13 19:50:46 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>

#define MAX_EVENTS 3
#define READ_SIZE 7

// int	epoll( int epfd, int op, int fd, struct epoll_event *event );
// epoll is used to add, modify, or remove entries for the "epfd"
// requests that operation "op" be performed for the target "fd"
// valid values are : EPOLL_CTL_ADD - EPOLL_CTL_MOD - EPOLL_CTL_DEL
int	main()
{
	struct	epoll_event	event, events[MAX_EVENTS];
	int	epoll_fd = epoll_create1( 0 );

	if ( epoll_fd == -1 )
	{
		fprintf( stderr, "Failed to create epoll fd\n" );
		return ( 1 );
	}
	
	event.events = EPOLLIN;
	event.data.fd = 0;

	if ( epoll_ctl( epoll_fd, EPOLL_CTL_ADD, 0, &event))
	{
		fprintf( stderr, "Failed to add file descriptor to epoll\n");
		close( epoll_fd );
		return ( 1 );
	}

	int	event_count = 0;
	size_t	bytes_read;
	char	read_buffer[READ_SIZE + 1];
	while ( true )
	{
		printf( "\nPolling for input..." );
		event_count = epoll_wait( epoll_fd, events, MAX_EVENTS, 30000 );
		printf( "%d ready events \n", event_count );
		for ( int i = 0; i < event_count; i++ )
		{
			printf( "Reading file descriptor num : [%d] --", events[i].data.fd );
			bytes_read = read( events[i].data.fd, read_buffer, READ_SIZE );
			printf( "%zd bytes read.\n", bytes_read );
			read_buffer[bytes_read] = '\0';
			printf( "Read '%s'\n",  read_buffer );
			if (!strncmp( read_buffer, "stop\n", 6))
				break ;
		}

	}

	if ( close ( epoll_fd ))
	{
		fprintf( stderr, "Failed to close epoll fd\n" );
		return ( 1 );
	}
	return ( 0 );
}
