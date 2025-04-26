/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:23 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/04/26 14:09:31 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RouteConfig.hpp"
#include "../includes/ServerConfig.hpp"
#include <climits>
#include <cstdlib>
#include <sstream>

#define MAX_SIZE_CLIENT 104857600

ServerConfig::ServerConfig() {}
ServerConfig::~ServerConfig() {}

void	ServerConfig::sentError(std::string msg) const
{	
	throw std::invalid_argument("<ServerConfig> : " + msg);
}

void	ServerConfig::setIpAndPort(std::string listen, std::string &ip, std::string &port)
{
	for (int i = 0; listen[i]; i++)
	{
		if (listen[i] == ':')
		{
			if (ip != "")
				sentError("Sintax error (listen) -> " + listen);
			ip = listen.substr(0, i);
			port = listen.substr(i + 1);
		}
	}
	if (ip == "")
	{
		ip = "0.0.0.0";
		port = listen;
	}
}

std::string	ServerConfig::getIp(in_addr_t inet)
{
	unsigned char	bytes[4];
	
	bytes[0] = (inet >> 24) & 0xFF;
	bytes[1] = (inet >> 16) & 0xFF;
	bytes[2] = (inet >> 8) & 0xFF;
	bytes[3] = inet & 0xFF;

	std::ostringstream	oss;

	oss <<	static_cast<int>(bytes[0]) << "."
		<<	static_cast<int>(bytes[1]) << "."
		<<	static_cast<int>(bytes[2]) << "."
		<<	static_cast<int>(bytes[3]);

	return oss.str();
}

in_addr_t	ServerConfig::getInet(std::string const &ip)
{
	unsigned char	bytes[4];
	size_t	pos = 0, lastPos = 0;

	for (int i = 0; i < 4; ++i)
	{
		pos = ip.find('.', lastPos);
		std::string	part = (pos == std::string::npos) ? ip.substr(lastPos) : ip.substr(lastPos, pos - lastPos);

		int	num = std::strtol(part.c_str(), NULL, 10);
		if (num < 0 || num > 255)
			sentError("The numbers of the ip have to be 0-255: " + part);

		bytes[i] = static_cast<unsigned char>(num);
		lastPos = pos + 1;
	}

	return	static_cast<in_addr_t>(bytes[0]) << 24 |
			static_cast<in_addr_t>(bytes[1]) << 16 |
			static_cast<in_addr_t>(bytes[2]) << 8 |
			static_cast<in_addr_t>(bytes[3]);
}

in_port_t	ServerConfig::getPort(std::string const &port)
{
	for (unsigned int i = 0; i < port.size(); i++)
		if (!std::isdigit(port[i]))
			sentError("The port has to be a positive number: " + port);
	
	if (port.size() > 6)
		sentError("The port is between 0 and 65535: " + port);

	int numPort = std::atoi(port.c_str());

	if (numPort < 0 || numPort > USHRT_MAX)
		sentError("The port is between 0 and 65535: " + port);
	
	return static_cast<in_port_t>(numPort);
}

void	ServerConfig::addListen(std::string listen)
{
	std::string ip, port;

	setIpAndPort(listen, ip, port);
	
	in_addr_t	numIp = getInet(ip);
	in_port_t	numPort = getPort(port);
	
	ListenMap::iterator	it = this->listen.find(numIp);
	if (it == this->listen.end())
	{
		std::vector<in_port_t>	tmp;
		tmp.push_back(numPort);
		this->listen.insert(std::make_pair(numIp, tmp));
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

bool	ServerConfig::getListenPorts(std::string ip, std::vector<in_port_t> ports)
{
	in_addr_t	inet = getInet(ip);

	ListenMap::iterator	it = listen.find(inet);
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

void	ServerConfig::addDefault()
{
	
}
