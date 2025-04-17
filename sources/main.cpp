/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:04:59 by avolcy            #+#    #+#             */
/*   Updated: 2025/04/17 19:35:40 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <csignal>
# include <iostream>
# include "../includes/Server.hpp"
# include "../includes/Config.hpp"

Server *serverPtr = nullptr;

void signalHandler(int signum)
{
    if (serverPtr)
        serverPtr->shutDownServer();
    exit(signum);
}

int	main()
{
	std::vector<ServerConfig> configs;
	std::vector<std::thread> threads;

	
	Server tela( AF_INET, SOCK_STREAM, 0 );
	serverPtr = &tela;
	
	signal(SIGINT, signalHandler);
	
	tela.start( INADDR_ANY, PORT, BACKLOG );
	tela.run();
	return ( 0 );
}
