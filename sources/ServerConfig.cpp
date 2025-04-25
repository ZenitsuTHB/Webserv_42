/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:23 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/04/25 20:19:35 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RouteConfig.hpp"
#include "../includes/ServerConfig.hpp"
#include <climits>
#include <cstdlib>

#define MAX_SIZE_CLIENT 104857600

ServerConfig::ServerConfig() {}
ServerConfig::~ServerConfig() {}

void	ServerConfig::sentError(std::string msg) const
{	
	throw std::invalid_argument("<ServerConfig> : " + msg);
}

void	ServerConfig::addListen(std::string ip, std::string port)
{
	for (unsigned int i = 0; i < port.size(); i++)
		if (!std::isdigit(port[i]))
			sentError("The port is a number");
	
	if (port.size() > 6)
		sentError("The port is between 0 and 65535");

	int numPort = std::atoi(port.c_str());

	if (numPort < 0 || numPort > USHRT_MAX)
		sentError("The port is between 0 and 65535");
	
	ListenMap::iterator	it = listen.find(ip);
	if (it == listen.end())
	{
		std::vector<int>	tmp;
		tmp.push_back(numPort);
		listen.insert(std::make_pair(ip, tmp));
	}
	else
		it->second.push_back(numPort);
}

void	ServerConfig::addServerName(std::string name)
{
	serverNames.push_back(name);
}

void	ServerConfig::addRoute(RouteConfig route)
{
	routes.push_back(route);
}

ListenMap const	&ServerConfig::getListen() const
{
	return listen;
}

bool	ServerConfig::getListenPorts(std::string ip, std::vector<int> ports)
{
	ListenMap::iterator	it = listen.find(ip);
	if (it == listen.end())
		return false;

	ports = it->second;

	return true;
}

std::vector<std::string> const	&ServerConfig::getServerNames() const
{
	return serverNames;
}

std::vector<RouteConfig> const	&ServerConfig::getRoutes() const
{
	return routes;
}

void	ServerConfig::display()
{
	std::cout << std::endl << "Server: " << serverNames[0] << std::endl << std::endl;

	for (size_t i = 0; i < serverNames.size(); i++)
		std::cout << "Server names: " << serverNames[i] << std::endl;
	
	for (ListenMap::iterator it = listen.begin(); it != listen.end(); it++)
		for (size_t i = 0; i < it->second.size(); i++)
			std::cout << "Listen ip: " << it->first << " port: " << it->second[i] << std::endl;
	
	std::cout << "Root: " << root << std::endl;
	
	for (size_t i = 0; i < indexFiles.size(); i++)
		std::cout << "IndexFile: " << indexFiles[i] << std::endl;
	
	for (ErrorMap::iterator	it = errorPages.begin(); it != errorPages.end(); it++)
		std::cout << "Error page " << it->first << ": " << it->second << std::endl;
	
	std::cout << "Return code: " << returnCode << std::endl;
	std::cout << "Return url: " << redirectUrl << std::endl;
	std::cout << "ClientMaxBody: " << clientMaxBodySize << std::endl;

	for (size_t i = 0; i < routes.size(); i++)
		routes[i].display();
}
