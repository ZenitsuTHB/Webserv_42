/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:09:31 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/06/01 15:34:47 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ParserConfig.hpp"
#include <sstream>

void	ParserConfig::sentError(std::string msg)
{
	throw std::invalid_argument("<ParserConfig> : " + msg);
}

ParserConfig::ParserConfig() {}

ParserConfig::~ParserConfig() {}

ParserConfig::ParserConfig(std::string input)
{
	// Check if is .conf
	
	if (input.size() < 5 || input.substr(input.size() - 5) != ".conf")
		sentError("Invalid configuration file (.conf) -> " + input);

	std::ifstream	file(input.c_str());

	if (!file.is_open())
		sentError("Fail to open -> " + input);

	// Main	program
	
	std::string		line;

	while (std::getline(file, line))
	{
		deleteComment(line);
		if (isEmpty(line))
			continue ;

		std::istringstream	iss(line);
		std::string	a, b;

		iss >> a >> b;
		
		if (a == "server" && b == "")
			addServer(file);
		else
			sentError("Syntax error: " + line);

		line.clear(); a.clear(); b.clear();
	}

	// Check if some server has the same ip-port tha other

	for (unsigned int i = 0; i < _servers.size(); i++)
	{
		in_addr_t	ip = _servers[i].getIp(); in_port_t	port = _servers[i].getPort();
		for (unsigned int j = 0; j < _servers.size(); j++)
			if (i != j && _servers[j].getIp() == ip && _servers[j].getPort() == port)
				sentError("You cannot have two servers with the same [ip]:[port]");
	}

	//for (size_t i = 0; i < _servers.size(); i++)
	//	_servers[i].display();
}

// Create a new server data

void	ParserConfig::addServer(std::ifstream &file)
{
	std::string	line, var, a;
	std::getline(file, line);
	std::istringstream	iss(line);
	ServerConfig	server;

	iss >> a;
	if (a != "{" || iss.fail())
		sentError("Syntax error: unexpected token: " + line);
	line.clear(); a.clear();

	while (std::getline(file, line))
	{
		deleteComment(line);
		if (isEmpty(line))
			continue ;

		VectorStr	values;

		getDataLine(line, var, values);

		if (var == "}")
			break ;
		else if (var == "location")
			addRoute(file, values, server);
		else if (values.size() != 0)
			addServerVar(var, values, server);
		else
			sentError("Syntax error: " + line);

		line.clear(); var.clear(); values.clear();
	}

	if (var != "}")
		sentError("Keys of the server not closed correctly");
	
	server.addDefault();
	_servers.push_back(server);
	
}

// Create a new route data

void	ParserConfig::addRoute(std::ifstream &file, VectorStr const &path, ServerConfig &server)
{
	std::string	line, var, a;
	std::getline(file, line);
	std::istringstream	iss(line);
	RouteConfig	route;

	route.setPath(path);

	iss >> a;
	if (a != "{" || iss.fail())
		sentError("Syntax error: unexpected token: " + line);
	line.clear(); a.clear();	

	while (std::getline(file, line))
	{
		deleteComment(line);
		if (isEmpty(line))
			continue ;	

		VectorStr	values;

		getDataLine(line, var, values);

		if (var == "}")
			break ;

		if (values.size() != 0)
			addRouteVar(var, values, route);
		else
			sentError("Syntax error: " + line);		

		line.clear(); var.clear(); values.clear();
	}
	
	if (var != "}")
		sentError("Keys of the route not closed correctly");
	server.addRoute(route);
}

// Add a variable into the server

void	ParserConfig::addServerVar(std::string const &var, VectorStr values, ServerConfig &server)
{
	if (var == "listen")
		server.setListen(values);
	else if (var == "server_name")
		server.setServerName(values);
	else if (var == "error_page")
		server.addErrorPage(values);
	else if (var == "client_max_body_size")
		server.setMaxSize(values);
	else if (var == "root")
		server.setRoot(values);
	else if (var == "backlog")
		server.setBacklog(values);
	else if (var == "index")
		server.addIndexFile(values);
	else if (var == "return" || var == "redirect")
		server.setReturn(values);
	else
		sentError("The directive " + var + " does not exists in this proyect!");
}

// Add a route variable into the route

void	ParserConfig::addRouteVar(std::string const &var, VectorStr values, RouteConfig &route)
{
	if (var == "root")
		route.setRoot(values);
	else if (var == "index")
		route.addIndexFile(values);
	else if (var == "autoindex")
		route.setAutoindex(values);
	else if (var == "limit_except" || var == "methods" || var == "allowed_methods")
		route.addMethods(values);
	else if (var == "error_page")
		route.addErrorPage(values);
	else if (var == "client_max_body_size")
		route.setMaxSize(values);
	else if (var == "return" || var == "redirect")
		route.setReturn(values);
	else if (var == "cgi_path")
		route.setCgiPath(values);
	else if (var == "cgi_extension")
		route.addCgiExtension(values);
	else if (var == "cgi_enable")
		route.setCgiEnable(values);
	else
		sentError("The directive " + var + " in the route does not exists in this proyect!");
}

// Check the line

void	ParserConfig::getDataLine(std::string line, std::string &var, VectorStr &values)
{
	std::istringstream	iss(line);
	std::string			val;

	// Get the name of the var or a key

	iss >> var;

	if (iss.fail())
		sentError("Unexpected error in istringstream: " + var);

	if (var == "{")
		sentError("Syntax error: unexpected token -> " + line);

	// Check if the line has more than one ';'
	std::string::size_type	first = line.find(';');
	if (first != std::string::npos)
	{
		if (var == "location")
			sentError("The routes does not have a ';' -> " + line);
		std::string::size_type	second = line.rfind(';');
		if (first != second)
			sentError("Syntax error: You have more than one ; -> " + line);
	}

	// Safe all the data except the first element into values

	while (iss >> val)
		values.push_back(val);

	// Check if there is a key and it is alone

	if (var == "}" && values.empty())
		return ;
	else if (var == "}")
		sentError("Syntax error: The keys alone please: " + line);

	// Delete ';' at the end of the line while the var is not a route

	if (values.empty() || values[0] == ";")
		sentError("Syntax error: Moooreeeeee!!: " + line);

	std::string last = values.back();
	if (var != "location" && last[last.length() - 1] != ';')
		sentError("Synatx error: the variables have to end with ; -> " + line);
	else if (var != "location")
	{
		last.erase(last.length() - 1);
		values.pop_back();
		if (!last.empty())
			values.push_back(last);
	}
}

// Check if the line is void or if it is full os space/tabs

bool	ParserConfig::isEmpty(std::string const &line) const
{
	if (line.empty())
		return true;

	for (unsigned int i = 0; i < line.length(); i++)
		if (!std::isspace(line[i]))
			return false;
	return true;
}

// Delete the comments of the line

void	ParserConfig::deleteComment(std::string &line)
{
	size_t	pos = line.find('#');

	if (pos != std::string::npos)
		line = line.substr(0, pos);
}

std::vector<ServerConfig>	ParserConfig::getServers() const
{
	return _servers;
}

unsigned int	ParserConfig::size() const
{
	return _servers.size();
}

// Displlay all the recollected servers info

void	ParserConfig::display()
{
	for (unsigned int i = 0; i < _servers.size(); i++)
		_servers[i].display();
}
