/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_socket.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 0025/02/21 22:47:19 by velazqu           #+#    #+#             */
/*   Updated: 2025/06/05 00:28:16 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int	main(void)
{
/*
=======================
Creation of a socket
=======================
*/
	/*	*
	 *	Creacion de socket
	 */
	int	socket_client;

	int	domain;
	int	type;
	int	protocol;

	domain = AF_INET;// Dominio para IPs (es una flag)
	type = SOCK_STREAM;// Estabecer un flujo de bytes
	protocol = 0;// Usa el protocolo por defecto al tipo

	socket_client = socket(domain, type, protocol);
	if (socket_client == -1)
		return (perror("Unsucked"), 1);

/*
=======================
Connection of socket to communication address
=======================
*/
	/*	*
	 *	Connectar el Socket a un Address
	 */
	struct sockaddr_in	address_client;

	address_client.sin_family = AF_INET;
	//address_client.sin_port = 8080; PUERTO TIENE QUE SE HTONS-ADO
	address_client.sin_port = htons(PORT);// Mismo puerto que el Server
	address_client.sin_addr.s_addr = INADDR_ANY;// Usa cualquier IP


	/*	*
	 *	Connexión con el Address
	 */
	int	connect_rtrn = connect(socket_client,
			(struct sockaddr *)&address_client, sizeof(struct sockaddr_in));
	if (connect_rtrn == -1)
		return (close(socket_client), perror("Cannot Connet Socket"), 2);

/*
=======================
Communication to Server
=======================
*/
	/*	*
	 *	Envio de mensaje al Server
	 */
	sleep(10);
	char	buffer[10240];
	int		rd_bytes;

	char	*message = "soy Gilipollas\r\n\r\n";

	int	write_rtrn = write(socket_client, message, strlen(message));
	if (write_rtrn == -1)
		return (close(socket_client), perror("No pude escribir"), 3);
	printf("Message send\n");

	/*	*
	 *	Lectura de respuesta del Server
	 */
	bzero(buffer, sizeof(buffer));
	rd_bytes = read(socket_client, buffer, sizeof(buffer));
	if (rd_bytes == -1)
		return (close(socket_client), perror("No pude leer :/"), 4);
	buffer[rd_bytes] = '\0';

	/*	*
	 *	Manejo de la respuesta del Server
	 */
	printf("Response:\n%s\n", buffer);
}
