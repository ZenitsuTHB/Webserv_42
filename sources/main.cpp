#include <iostream>
#include <vector>
#include <csignal>
#include <cstdlib>
#include <unistd.h>
#include "../includes/Server.hpp"
#include "../includes/ServerManager.hpp"
#include "../includes/ParserConfig.hpp"

ServerManager* serverManagerPtr = NULL;

void signalHandler(int signum) {
	if (serverManagerPtr)
		serverManagerPtr->stop();
	exit(signum);
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
		signal(SIGINT, signalHandler);

		std::cout << "[MAIN] Starting " << configs.size() << " servers..." << std::endl;
		manager.run();

		std::cout << "[MAIN] Servers shut down gracefully." << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Fatal Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
