/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:09:31 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/01 18:37:07 by adrmarqu         ###   ########.fr       */
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
	//	std::cout << i << "- " <<  _configFile[i] << std::endl;

	parserData();

	for (size_t i = 0; i < _servers.size(); i++)
		_servers[i].display();
}

bool	ParserConfig::isEmpty(std::string const &line) const
{
	if (line.empty())
		return true;

	for (unsigned int i = 0; i < line.length(); i++)
		if (!std::isspace(line[i]))
			return false;
	return true;
}

void	ParserConfig::setLine(std::string &line)
{
	unsigned int i = 0;
	
	while (std::isspace(line[i]))
		i++;

	line.erase(0, i);

	char	c = line[line.length() - 1];

	line.erase(line.length() - 1);

	if (c == ';')
		_configFile.push_back(line);
	else if (c == '{' || c == '}')
	{
		if (!isEmpty(line))
			_configFile.push_back(line);
		
		std::string key(1, c);
		_configFile.push_back(key);
	}
	else
		sentError("Sintax error: " + line);
}

void	ParserConfig::setDataFile(std::ifstream &file)
{
	std::string	line;

	while (std::getline(file, line))
	{
		if (isEmpty(line))
			continue ;
		setLine(line);
		line.clear();
	}
}

void	ParserConfig::parserData()
{
	if (_configFile.size() < 2)
		sentError("Error: Content of the file is not correct!");
	
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
		std::string	var = getVar(_configFile[i]);

		if (var == "location")
			addRoute(i, server);
		else if (var == "}")
			break ;
		else
			addServerVar(_configFile[i], var, server);
		var.clear();
	}

	server.addDefault();
	_servers.push_back(server);
}

void	ParserConfig::addServerVar(std::string line, std::string const &var, ServerConfig &server)
{
	std::vector<std::string>	values = getValues(line);
	
	if (values.size() == 0)
		sentError("Sintax error: " + var);

	if (var == "listen")
		server.addListen(values);
	else if (var == "server_name")
		server.addServerName(values);
	else if (var == "error_page")
		server.addErrorPage(values);
	else if (var == "client_max_body_size")
		server.setMaxSize(values);
	else if (var == "root")
		server.setRoot(values);
	else if (var == "index")
		server.addIndexFile(values);
	else if (var == "return" || var == "redirect")
		server.setReturn(values);
	else
		sentError("The directive " + var + " does not exists in this proyect");
}

void	ParserConfig::addRoute(unsigned int &i, ServerConfig &server)
{
	RouteConfig	route;
	
	std::vector<std::string> path = getValues(_configFile[i++]);

	route.setPath(path);

	if (_configFile[i++] != "{")
		sentError("Sintax error ('{'): " + _configFile[i - 1]);

	for (; i < _configFile.size(); i++)
	{
		std::string	var = getVar(_configFile[i]);
		if (var == "}")	
			break ;
		addRouteVar(_configFile[i], var, route);
		var.clear();
	}
	
	route.addDefault();
	server.addRoute(route);
}

void	ParserConfig::addRouteVar(std::string line, std::string const &var, RouteConfig &route)
{
	std::vector<std::string>	values = getValues(line);

	if (values.size() == 0)
		sentError("Sintax error: " + var);

	if (var == "root")
		route.setRoot(values);
	else if (var == "index")
		route.addIndexFile(values);
	else if (var == "autoindex")
		route.setAutoIndex(values);
	else if (var == "limit_except" || var == "methods" || var == "allowed_methods")
		route.addMethods(values);
	else if (var == "error_page")
		route.addErrorPage(values);
	else if (var == "client_max_body_size")
		route.setMaxSize(values);
	else if (var == "return" || var == "redirect")
		route.setReturn(values);
	else if (var == "cgi_pass" || var == "cgi_path" || var == "fastcgi_pass")
		route.setCgiPass(values);
	else if (var == "cgi_extension")
		route.addCgiExtension(values);
	else
		sentError("The directive " + var + " in the route does not exists in this proyect");
}

std::vector<ServerConfig> const		&ParserConfig::getServers() const
{
	return _servers;
}

std::string	ParserConfig::getVar(std::string const &line)
{
	std::string first;

	for (unsigned int i = 0; i < line.size(); i++)
	{
		if (std::isspace(line[i]))
			break ;
		first += line[i];
	}
	return first;
}

std::vector<std::string>	ParserConfig::getValues(std::string const &line)
{
	std::istringstream			iss(line);
	std::string					element;
	std::vector<std::string>	vec;

	iss >> element;
	while (iss >> element)
		vec.push_back(element);
	return vec;
}
