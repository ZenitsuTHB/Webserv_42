/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:09:31 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/04/26 14:09:57 by adrmarqu         ###   ########.fr       */
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

	for (size_t i = 0; i < _servers.size(); i++)
		_servers[i].display();
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
	for (unsigned int i = 0; i < _configFile.size(); i++)
	{
		if (_configFile[i] == "server" && _configFile[i + 1] == "{")
			addServer(i += 2);
		else
			sentError("Sintax error: " + _configFile[i] + _configFile[i + 1]);
	}
}

void	ParserConfig::addServer(unsigned int &i)
{
	ServerConfig	server;
	
	for (; i < _configFile.size(); i++)
	{
		if (_configFile[i] == "location")
			addRoute(i += 1, server);
		else if (_configFile[i] == "}")
			break ;
		else
			addServerVar(i, server);
	}
	server.addDefault();
	_servers.push_back(server);
}

void	ParserConfig::addServerVar(unsigned int &i, ServerConfig &server)
{
	std::string	var = _configFile[i++];

	if (var == "listen")
		server.addListen(_configFile[i++]);
	else if (var == "server_name")
	{
		for (; i < _configFile.size() && _configFile[i] != ";"; i++)
			server.addServerName(_configFile[i]);
	}
	else if (var == "error_page")
	{
		std::vector<std::string>	errors;

		for (; i < _configFile.size() && _configFile[i] != ";"; i++)
			errors.push_back(_configFile[i]);

		for (unsigned int j = 0; j < errors.size() - 1; j++)
			server.addErrorPage(errors[j], errors.back());
	}
	else if (var == "client_max_body_size")
		server.setMaxSize(_configFile[i++]);
	else if (var == "root")
		server.setRoot(_configFile[i++]);
	else if (var == "index")
		for (; i < _configFile.size() && _configFile[i] != ";"; i++)
			server.addIndexFile(_configFile[i]);
	else if (var == "return" || var == "redirect")
	{
		server.setReturn(_configFile[i], _configFile[i + 1]);
		i += 2;
	}
	else
		sentError("The directive " + var + " does not exists in this proyect");

	if (_configFile[i] != ";")
		sentError("Sintax error (var values;): " + _configFile[i]);
}

void	ParserConfig::addRoute(unsigned int &i, ServerConfig &server)
{
	RouteConfig	route;

	route.setPath(_configFile[i++]);

	if (_configFile[i++] != "{")
		sentError("Sintax error ('{'): " + _configFile[i - 1]);

	for (; i < _configFile.size(); i++)
	{
		if (_configFile[i] == "}")	
			break ;
		addRouteVar(i, route);
	}
	route.addDefault();
	server.addRoute(route);
}

void	ParserConfig::addRouteVar(unsigned int &i, RouteConfig &route)
{
	std::string	var = _configFile[i++];

	if (var == "root")
		route.setRoot(_configFile[i++]);
	else if (var == "index")
		for (; i < _configFile.size() && _configFile[i] != ";"; i++)
			route.addIndexFile(_configFile[i]);
	else if (var == "autoindex")
		route.setAutoIndex(_configFile[i++]);
	else if (var == "limit_except" || var == "methods" || var == "allowed_methods")
		for (; i < _configFile.size() && _configFile[i] != ";"; i++)
			route.addMethod(_configFile[i]);
	else if (var == "error_page")
	{
		std::vector<std::string>	errors;

		for (; i < _configFile.size() && _configFile[i] != ";"; i++)
			errors.push_back(_configFile[i]);

		for (unsigned int j = 0; j < errors.size() - 1; j++)
			route.addErrorPage(errors[j], errors.back());
	}
	else if (var == "client_max_body_size")
		route.setMaxSize(_configFile[i++]);
	else if (var == "return" || var == "redirect")
	{
		route.setReturn(_configFile[i], _configFile[i + 1]);
		i += 2;
	}
	else if (var == "cgi_pass" || var == "cgi_path" || var == "fastcgi_pass")
		route.setCgiPass(_configFile[i++]);
	else if (var == "cgi_extension")
		for (; i < _configFile.size() && _configFile[i] != ";"; i++)
			route.addCgiExtension(_configFile[i]);
	else
		sentError("The directive " + var + " in the route does not exists in this proyect");
	
	if (_configFile[i] != ";")
		sentError("Sintax error (var values;): " + _configFile[i]);
}

std::vector<ServerConfig> const		&ParserConfig::getServers() const
{
	return _servers;
}
