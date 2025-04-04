/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:04:59 by avolcy            #+#    #+#             */
/*   Updated: 2025/04/04 18:00:35 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <csignal>
# include <iostream>
# include "../includes/Server.hpp"

Server *serverPtr = nullptr;

void signalHandler(int signum)
{
    if (serverPtr)
        serverPtr->shutDownServer();
    exit(signum);
}

int	main()
{
	Server tela( AF_INET, SOCK_STREAM, 0 );
	
	serverPtr = &tela;
	
	signal(SIGINT, signalHandler);
	
	tela.start( INADDR_ANY, PORT, BACKLOG );

	std::cout << "Serveur démarré sur le port " << PORT << std::endl;
	
	tela.run();
	return ( 0 );
}
