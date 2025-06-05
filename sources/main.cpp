/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:55:47 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/06/05 16:47:02 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <csignal>
#include <cstdlib>
#include <unistd.h>
#include "../includes/Server.hpp"
#include "../includes/ServerManager.hpp"
#include "../includes/ParserConfig.hpp"

ServerManager* serverManagerPtr = NULL;
volatile sig_atomic_t g_shouldStop = 0;  // ✅ Initialization here only

void signalHandler(int signum) {
	(void)signum;
	g_shouldStop = 1;
	// if (serverManagerPtr)
	// 	serverManagerPtr->stop();
	// exit(signum);
}

int main(int ac, char **av)
{
	if (ac != 2) {
		std::cerr << "Error: ./webserv [Configuration file]" << std::endl;
		return 1;
	}

	try {
		ParserConfig parser(av[1]);
		const std::vector<ServerConfig>& configs = parser.getServers();

		if (configs.empty()) {
			std::cerr << "Error: No servers defined in config file." << std::endl;
			return 1;
		}
		

		ServerManager manager(configs);
		serverManagerPtr = &manager;
		if (!manager.getServerCount()) {
			std::cerr << "[MAIN] No servers started successfully. Exiting." << std::endl;
			return 1;
		}		
		std::cout << "[MAIN] Starting " << configs.size() << " servers..." << std::endl;
		signal(SIGINT, signalHandler);
		signal(SIGTERM, signalHandler);
		signal(SIGHUP, signalHandler);
		manager.run();

		std::cout << "[MAIN] Servers shut down gracefully." << std::endl;
	}
	catch (std::exception const &e)
	{
		std::cerr << "Error in child process: " << e.what() << std::endl;
		//exit(1);
	}
	return 0;
}

