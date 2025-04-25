/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:09:31 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/04/25 14:45:19 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ParserConfig.hpp"
#include "../includes/ServerConfig.hpp"
#include <fstream>
#include <sstream>

void	ParserConfig::sentError(std::string msg)
{
	throw std::invalid_argument("<ParserConfig> : " + msg);
}

ParserConfig::~ParserConfig() {}

ParserConfig::ParserConfig(std::string input)
{
	if (input.size() < 5 || input.substr(input.size() - 5) != ".conf")
		sentError("Invalid input -> " + input);
	
	std::ifstream	file(input.c_str());

	if (!file.is_open())
		sentError("Fail to open -> " + input);

	setDataFile(file);
	file.close();

	//for (size_t i = 0; i < _configFile.size(); i++)
	//	std::cout << _configFile[i] << std::endl;

	parserData();

}

void	ParserConfig::setDataFile(std::ifstream &file)
{
	std::string	line;
	std::string	data;
	std::string	token;
	size_t		pos;

	while (std::getline(file, line))
		data += line;

	std::istringstream	iss(data);

	while (iss >> token)
	{
		pos = 0;
		while ((pos = token.find_first_of("{};=")) != std::string::npos)
		{
			if (pos > 0)
				_configFile.push_back(token.substr(0, pos));
			_configFile.push_back(token.substr(pos, 1));
			token = token.substr(pos + 1);
		}
		if (!token.empty())
			_configFile.push_back(token);
	}
}

void	ParserConfig::parserData()
{
	for (int i = 0; i < static_cast<int>(_configFile.size()); i++)
	{
		if (_configFile[i] == "server" && _configFile[i + 1] == "{")
			addServer(i += 2);
		else
			sentError("Sintax error: " + _configFile[i] + _configFile[i + 1]);
	}
}

void	ParserConfig::addServer(int &i)
{
	ServerConfig	server;
	

	std::cout << _configFile[i] << std::endl;

	for (; i < static_cast<int>(_configFile.size()); i++)
	{
		if (_configFile[i] == "location")
			addRoute(i += 1, server);
		else if (_configFile[i] == "}")
			break ;
		else
			addServerVar(i, server);
	}
	_servers.push_back(server);
}

void	ParserConfig::addRoute(int &i, ServerConfig server)
{
	RouteConfig	route;

	route.setPath(_configFile[i++]);

	if (_configFile[i++] != "{")
		sentError("Sintax error ('{'): " + _configFile[i - 1]);

	for (; i < static_cast<int>(_configFile.size()); i++)
	{
		if (_configFile[i] == "}")	
			break ;
		addRouteVar(i, route);
	}

	server.addRoute(route);
}

void	ParserConfig::addServerVar(int &i, ServerConfig server)
{
	std::string	var = _configFile[i++];

	// Buscar variable
	
	/*if (var == "listen")
		server.addListen(_configFile[i++]);
	else if (var == "server_name")
		server.addServerName();
	else if (var == "error_page")
		server.addErrorPage();
	else if (var == "client_max_body_size")
		server.setMaxSize(_configFile[i++]);
	else if (var == "root")
		server.setRoot(_configFile[i++]);
	else if (var == "index")
		server.addIndexFile();
	else if (var == "return" || var == "redirect")
		server.setReturn();
	else
		sentError("The directive " + var + " does not exists in this proyect");
*/
	
	//if (_configFile[i] != ";")
	//	sentError("Sintax error (var value;): " + var + value + _configFile[i]);
	
	(void)server;
}

void	ParserConfig::addRouteVar(int &i, RouteConfig route)
{
	std::cout << "Route var: " << _configFile[i] << std::endl;
	i++;
	(void)route;
}

std::vector<ServerConfig> const		&ParserConfig::getServers() const
{
	return _servers;
}
