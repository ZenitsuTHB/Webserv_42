/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:23 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/10 20:29:16 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerConfig.hpp"
#include <climits>
#include <cstdlib>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>

ServerConfig::ServerConfig(): backlog(-1) {}
ServerConfig::~ServerConfig() {}

void	ServerConfig::sentError(std::string msg) const
{	
	throw std::invalid_argument("<ServerConfig> : " + msg);
}

// Look into /etc/hosts for an ip and returns it

std::string	ServerConfig::lookForHost(std::string const &hostname)
{
	int	fd = open("/etc/hosts", O_RDONLY);
	if (fd < 0)
		sentError("Error to open /etc/hosts");

	char		buffer[4096];
	ssize_t		bytesRead;
	std::string	line;

	while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0)
	{
		for (ssize_t i = 0; i < bytesRead; i++)
		{
			if (buffer[i] == '\n')
			{	
				if (line.find(hostname) != std::string::npos)
					return close(fd), line;
				line.clear();
			}
			else
				line += buffer[i];
		}
	}
	return close(fd), line;
}

// Converts a unsigned int ip to std::string (0.0.0.0) ip

std::string	ServerConfig::convertIp(in_addr_t inet)
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

// Converts an ip (0.0.0.0) to a number

in_addr_t	ServerConfig::getInet(std::string const &ip)
{
	unsigned char	bytes[4];
	size_t			pos = 0, lastPos = 0;
	std::string		part;

	for (unsigned int i = 0; i < ip.length(); i++)
		if (!std::isdigit(ip[i]) && ip[i] != '.')
			sentError("The ip has to be a number: " + ip);

	for (int i = 0; i < 4; ++i)
	{
		pos = ip.find('.', lastPos);

		if (pos == std::string::npos)
			part = ip.substr(lastPos, pos);
		else
			part = ip.substr(lastPos);

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

// Converts a std::string port to a number port

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

// listen (optional)[address]:[port]
// default: [0.0.0.0][80], [0.0.0.0][8000]

void	ServerConfig::setListen(VectorStr const &data)
{
	if (data.size() != 1)
		sentError("Syntax error: listen -> listen [ip]:[port]");

	if (ip != "")
		sentError("You cannot have multiple listen");

	std::string	listen = data[0];
	size_t		pos = listen.find(':');

	if (pos == std::string::npos)
	{
		this->ip = "0.0.0.0";
		this->ipNum = getInet(this->ip);
		this->port = getPort(listen);
		return ;
	}
	std::string	host = listen.substr(0, pos);
	std::string	port = listen.substr(pos + 1);

	std::string	ip = lookForHost(host);

	if (!ip.empty())
	{
		host.clear();
		for (unsigned int i = 0; i < ip.length(); i++)
		{
			if (std::isspace(ip[i]))
				break ;
			host += ip[i];
		}
	}
	this->ipNum = getInet(host);
	this->ip = host;
	this->port = getPort(port);
}

// server_name [name]
// default: "";

void	ServerConfig::setServerName(VectorStr const &data)
{
	if (data.size() != 1)
		sentError("Syntax error: multiple server name -> server_name [name]");
	if (serverName != "")
		sentError("You cannot have multiple server_name");

	serverName = data[0];
}

void	ServerConfig::setBacklog(VectorStr const &data)
{
	if (data.size() != 1)
		sentError("Syntax error: backlog -> backlog [value]");

	for (unsigned int i = 0; i < data[0].length(); i++)
		if (!std::isdigit(data[0][i]))
			sentError("The backlog has to be a positive number: " + data[0]);

	this->backlog = std::atoi(data[0].c_str());
}

void	ServerConfig::addRoute(RouteConfig route)
{
	routes.push_back(route);
}

std::string const	&ServerConfig::getIp() const
{
	return ip;
}

in_addr_t	ServerConfig::getIpNum() const
{
	return ipNum;
}

in_port_t	ServerConfig::getPortNum() const
{
	return port;
}

std::string const	&ServerConfig::getServerName() const
{
	return serverName;
}

RouteConfig const	&ServerConfig::getRoute(int route) const
{
	return routes[route];
}

std::vector<RouteConfig> const	&ServerConfig::getRoutes() const
{
	return routes;
}

unsigned int	ServerConfig::getNumRoutes() const
{
	return routes.size();
}

int	ServerConfig::getBacklog() const
{
	return backlog;
}

// Add the rest of the data that is not in the configuration file

void	ServerConfig::addDefault()
{
	if (root.empty())
		root = "html";
	if (indexFiles.empty())
		indexFiles.push_back("index.html");
	if (clientMaxBodySize == 0)
		setMaxSize(MAX_SIZE_CLIENT);
	if (ip.empty())
	{
		ip = "0.0.0.0";
		ipNum = 0;
		port = 80;
	}
	if (serverName.empty())
		serverName = "default";

	if (backlog == -1)
		backlog = 100;

	bool	therIs = false;
	for (unsigned int i = 0; i < routes.size(); i++)
	{
		routes[i].addDefault();
		if (routes[i].getPath() == "/" && !therIs)
			therIs = true;
	}

	// If the route root does not exists, then is created

	if (!therIs)
	{
		RouteConfig	newRoute;

		newRoute.addDefault();
		routes.push_back(newRoute);
	}

	// Check if there are two same routes

	for (unsigned int i = 0; i < routes.size(); i++)
		for (unsigned int j = 0; j < routes.size(); j++)
			if (i != j && routes[i].getPath() == routes[j].getPath())
				sentError("The routes are unique: " + routes[i].getPath());
}

// Display all the data of the server

void	ServerConfig::display()
{
	std::cout << std::endl << "Server: " << serverName << std::endl << std::endl;

	std::cout << "Listen ip: " << ip << ", port: " << port << std::endl;
	std::cout << "ip number: " << ipNum << std::endl;
	std::cout << "backlog: " << backlog << std::endl;
	
	std::cout << "Root: " << root << std::endl;
	
	for (size_t i = 0; i < indexFiles.size(); i++)
		std::cout << "IndexFile: " << indexFiles[i] << std::endl;
	
	if (!errorPages.empty())
		for (ErrorMap::iterator	it = errorPages.begin(); it != errorPages.end(); it++)
			std::cout << "Error page " << it->first << ": " << it->second << std::endl;
	
	std::cout << "Return code: " << returnCode << std::endl;
	std::cout << "Return url: " << redirectUrl << std::endl;
	std::cout << "ClientMaxBody: " << clientMaxBodySize << std::endl;

	for (size_t i = 0; i < routes.size(); i++)
		routes[i].display();
}
