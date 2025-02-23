/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  server_socket.c                                      :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/02/21 22:43:49 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/02/21 22:54:26 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int	main(void)
{
/*
=======================
Creation of a socket
=======================
*/
	int	socket_server;

	int	domain;
	int	type;
	int	protocol;

	domain = AF_INET;// IPv4 Internet Protocols
	type = SOCK_STREAM;// Fluzo de bytes secuenciado, doble...  RTFM(socket)!
	protocol = 0;// Protocolo por defecto al tipo de socket

	/*	*
	 *	Creando el socket (es un fd)
	 *	sk_fd = socket(AF_INET, SOCK_STREAM, 0);*/
	socket_server = socket(domain, type, protocol);
	if (socket_server == -1)
		return (perror("Sucked not created\n"), 1);

/*
=======================
The Binding of Socket
=======================
*/
	/*	*
	 *	struct sockaddr_in
	 *	{
	 *		sa_family_t		sin_family;// Un integer, determina tipo
	 *		in_port_t		sin_port;// Un integer, puerto
	 *		struct in_addr	sin_addr
	 *		{
	 *			in_addr_t	s_addr;// El IP (Un integer)
	 *		};
	 *	}*/
	struct sockaddr_in	address_server;
	int					port;
	int					ip_address;

	port = 8080;// No se, este num me gusta
	ip_address = INADDR_ANY;// Asignar automaticamente la ip, misma del pc

	bzero(&address_server, sizeof(struct sockaddr_in));
	address_server.sin_family = domain;// AF_INET
	address_server.sin_port = htons(port);// 8080
	address_server.sin_addr.s_addr = htonl(ip_address);// 0.0.0.0

	/*	*
	 *	Uniendo el socket a una dirección ip
	 *	y un puerto
	 */
	int	bind_rtrn = bind(socket_server, (struct sockaddr *)&address_server,
			sizeof(struct sockaddr_in));
	if (bind_rtrn == -1)
		return (perror("The Binding of Fails"), 2);

/*
=======================
Make the socket a passive socket (Server socket)
=======================
*/
	/*	*
	 *	Preparar el socket para aceptar solicitudes de conexión
	 *	10 = numero máximo de la lista de conexiones pendientes
	 */
	struct sockaddr_storage	address_request;
	int						socket_request;
	socklen_t				addr_len;
	char					buffer[10240];
	int						rd_bytes;

	char	*message = "Hola Mundo, soy el SERVER\n";

	int	listen_rtrn = listen(socket_server, 10);
	if (listen_rtrn == -1)
		return (perror("Lisenin desaprobaoh"), 3);

	/*	*
	 *	Espera de solicitudes y bucle general de
	 *	Aceptar - Administrar - Responder
	 */
	int	i = 0;
	while (i < 10)
	{
		addr_len = sizeof(struct sockaddr_storage);
		bzero(buffer, sizeof(buffer));
		/*	*
		 *	Admisión
		 */
		printf("\n##### Waiting for request #####\n");
		socket_request = accept(socket_server,
				(struct sockaddr *)&address_request, &addr_len);
		if (socket_request == -1)
			break ;
		rd_bytes = read(socket_request, buffer, sizeof(buffer));
		if (rd_bytes == -1)
		{
			close(socket_request);
			break ;
		}
		buffer[rd_bytes] = '\0';
		/*	*
		 *	Manejo
		 */
		printf("\n##### Request handled #####\n- Content:\n%s\n", buffer);
		/*	*
		 *	Respuesta
		 */
		write(socket_request, message, strlen(message));
		close(socket_request);
		printf("\n##### Response sent #####\n");
		i++;
	}

	close(socket_server);
	return (0);
}
