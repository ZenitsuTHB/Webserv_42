/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:04:59 by avolcy            #+#    #+#             */
/*   Updated: 2025/04/25 13:51:41 by adrmarqu         ###   ########.fr       */
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


struct ThreadArg {
	ServerConfi config;
};

extern "C" void* serverLauncher(void* arg) {
	ThreadArg* tArg = static_cast<ThreadArg*>(arg);
	Server server(AF_INET, SOCK_STREAM, 0);
	serverPtr = &server;
	signal(SIGINT, signalHandler);
	std::cout << "[THREAD] Starting server on port " << tArg->config.port << std::endl;
	server.start(tArg->config.ip, tArg->config.port, tArg->config.backlog);
	server.run();
	return NULL;
}

int main(int ac, char **av)
{
	if (ac != 2)
		return std::cerr << "Error: ./webserv [Configuration file]" << std::endl, 1;

	try
	{
		ParserConfig	data(av[1]);
	}
	catch (std::exception const &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}


	/*
	ServerConfig configs[] = {
		{INADDR_ANY, 8080, 100},
		{INADDR_ANY, 8081, 100},
		{INADDR_ANY, 8082, 100}
	};

	const size_t NUM_SERVERS = sizeof(configs) / sizeof(ServerConfig);
	pthread_t threads[NUM_SERVERS];
	ThreadArg args[NUM_SERVERS];

	for (size_t i = 0; i < NUM_SERVERS; ++i) {
		args[i].config = configs[i];
		if (pthread_create(&threads[i], NULL, serverLauncher, &args[i]) != 0) {
			std::cerr << "Failed to launch thread for port " << configs[i].port << std::endl;
		}
	}

	for (size_t i = 0; i < NUM_SERVERS; ++i)
		pthread_join(threads[i], NULL);

	std::cout << "[MAIN] All servers terminated.\n";
	*/
	return 0;
}
