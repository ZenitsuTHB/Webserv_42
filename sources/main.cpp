/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:04:59 by avolcy            #+#    #+#             */
/*   Updated: 2025/05/16 18:37:15 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <csignal>
#include <cstdlib>
#include <pthread.h>
#include "../includes/Server.hpp"
#include "../includes/Config.hpp"
#include "../includes/ParserConfig.hpp"

Server *serverPtr = nullptr;

void signalHandler(int signum)
{
	if (serverPtr)
		serverPtr->shutDownServer();
	exit(signum);
}
/*
extern "C" void* serverLauncher(void* arg)
{
	try
	{
	ServerConfig	*obj = static_cast<ServerConfig*>(arg);
	Server		server(AF_INET, SOCK_STREAM, 0);

	serverPtr = &server;
	
	signal(SIGINT, signalHandler);
	
	std::cout << "[THREAD] Starting server on port " << obj->getPortNum() << std::endl;
	
	server.start(obj->getIpNum(), obj->getPortNum(), obj->getBacklog());
	server.run();
	
	}
	catch (std::exception const &ex)
	{
		std::cerr << "Me cago en todo 2 " << ex.what() << std::endl;
	}
	return NULL;
}
*/
void	launchServer(ServerConfig obj)
{
	try
	{
		// Hacer fork()

		Server	server(obj.getIpNum(), obj.getPortNum(), obj.getBacklog());

		serverPtr = &server;

		signal(SIGINT, signalHandler);
		std::cout << "Starting serevr in port: " << obj.getPortNum() << std::endl;

		server.run();
	}
	catch (std::exception const &e)
	{
		std::cerr << "Error in server " << obj.getPortNum() << " -> " << e.what() << std::endl;
	}
}

int main(int ac, char **av)
{
	if (ac != 2)
		return std::cerr << "Error: ./webserv [Configuration file]" << std::endl, 1;

	try
	{
		std::vector<ServerConfig>	servers;
		
		ParserConfig	parser(av[1]);
		servers = parser.getServers();

		for (unsigned int i = 0; i < servers.size(); i++)
			launchServer(servers[i]);
	}
	catch (std::exception const &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
/*
	std::cout << "No es mi problema es de Ash ly de pueblo paleta" << std::endl;
	try
	{

	pthread_t	threads[servers.size()];

	for (unsigned int i = 0; i < servers.size(); i++)
	{
		void	*dir = static_cast<void*>(&servers[i]);
		if (pthread_create(&threads[i], NULL, serverLauncher, dir) != 0)
			std::cerr << "Failed to launch thread for port " << servers[i].getPortNum() << std::endl;
	}
	
	for (unsigned int i = 0; i < servers.size(); i++)
		pthread_join(threads[i], NULL);
	
	std::cout << "[MAIN] All servers terminated.\n";
	}
	catch (std::exception const &ex)
	{
		std::cerr << "Me cago en todo " << ex.what() << std::endl;
	}
*/
	return 0;
}
