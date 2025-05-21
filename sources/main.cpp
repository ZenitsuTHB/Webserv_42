/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:55:47 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/21 17:55:57 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <csignal>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include "../includes/Server.hpp"
#include "../includes/Config.hpp"
#include "../includes/ParserConfig.hpp"

Server *serverPtr = nullptr;

void	signalHandler(int signum)
{
	if (serverPtr)
		serverPtr->shutDownServer();
	exit(signum);
}

void	launchServer(ServerConfig const &obj)
{
	try
	{
		Server server(obj.getIpNum(), obj.getPortNum(), obj.getBacklog());
		
		serverPtr = &server;
		signal(SIGINT, signalHandler);

		std::cout << "[PROCESS] Starting server on port " << obj.getPortNum() << std::endl;
		server.run();
	}
	catch (std::exception const &e)
	{
		std::cerr << "Error in child process: " << e.what() << std::endl;
		//exit(1);
	}
}

int	main(int ac, char **av)
{
	if (ac != 2)
		return std::cerr << "Error: ./webserv [Configuration file]" << std::endl, 1;

	try
	{
		ParserConfig					parser(av[1]);
		std::vector<ServerConfig> const	&server = parser.getServers();
		std::vector<pid_t> 				pids;

		std::cout << "X: " << server[0].getRoute("////images///xd//")->getPath() << std::endl;
		for (size_t i = 0; i < server.size(); i++)
			launchServer(server[i]);

		/*for (size_t i = 0; i < server.size(); i++)
		{
			pid_t	pid = fork();
			
			if (pid < 0)
				std::cerr << "Fork failed" << std::endl;
			else if (pid == 0)
			{
				launchServer(server[i]);
				exit(0);
			}
			else
				pids.push_back(pid);
		}
		
		for (size_t i = 0; i < pids.size(); i++)
		{
			int	status;
			waitpid(pids[i], &status, 0);
			std::cout << "Server process " << pids[i] << " exited with " << status << std::endl;
		}
		*/
		
		std::cout << "[MAIN] All servers terminated.\n";
	}
	catch (std::exception const &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
