/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:09:31 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/08 14:30:42 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ParserConfig.hpp"
#include <sstream>

void	ParserConfig::sentError(std::string msg)
{
	throw std::invalid_argument("<ParserConfig> : " + msg);
}

ParserConfig::~ParserConfig() {}

ParserConfig::ParserConfig(std::string input)
{
	if (input.size() < 5 || input.substr(input.size() - 5) != ".conf")
		sentError("Invalid configuration file (.conf) -> " + input);

	std::ifstream	file(input.c_str());

	if (!file.is_open())
		sentError("Fail to open -> " + input);

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

	for (unsigned int i = 0; i < _servers.size(); i++)
	{
		in_addr_t	ip = _servers[i].getIpNum(); in_port_t	port = _servers[i].getPortNum();
		for (unsigned int j = 0; j < _servers.size(); j++)
			if (i != j && _servers[j].getIpNum() == ip && _servers[j].getPortNum() == port)
				sentError("You cannot have two servers with the same [ip]:[port]");
	}

	for (size_t i = 0; i < _servers.size(); i++)
		_servers[i].display();
}

void	ParserConfig::addServer(std::ifstream &file)
{
	std::string	line, a;
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
		std::string	var;

		getDataLine(line, var, values);

		if (var == "}")
			break ;
		else if (var == "location" && values.size() == 1)
			addRoute(file, values, server);
		else if (values.size() != 0)
			addServerVar(var, values, server);
		else
			sentError("Syntax error: " + line);

		line.clear(); var.clear(); values.clear();
	}
	
	server.addDefault();
	_servers.push_back(server);
}

void	ParserConfig::addRoute(std::ifstream &file, VectorStr const &path, ServerConfig &server)
{
	std::string	line, a;
	std::getline(file, line);
	std::istringstream	iss(line);
	RouteConfig	route;

	iss >> a;
	if (a != "{" || iss.fail())
		sentError("Syntax error: unexpected token: " + line);
	line.clear(); a.clear();
	
	if (path.size() != 1)
		sentError("Syntax error: you need a path in a location");

	route.setPath(path[0]);

	while (std::getline(file, line))
	{
		deleteComment(line);
		if (isEmpty(line))
			continue ;	

		VectorStr	values;
		std::string	var;

		getDataLine(line, var, values);

		if (var == "}")
			break ;

		if (values.size() != 0)
			addRouteVar(var, values, route);
		else
			sentError("Syntax error: " + line);		

		line.clear(); var.clear(); values.clear();
	}
	
	server.addRoute(route);
}

void	ParserConfig::addServerVar(std::string const &var, VectorStr values, ServerConfig &server)
{
	if (var == "listen")
		server.setListen(values[0]);
	else if (var == "server_name")
		server.setServerName(values[0]);
	else if (var == "error_page")
		server.addErrorPage(values);
	else if (var == "client_max_body_size")
		server.setMaxSize(values[0]);
	else if (var == "root")
		server.setRoot(values[0]);
	else if (var == "index")
		server.addIndexFile(values);
	else if (var == "return" || var == "redirect")
		server.setReturn(values);
	else
		sentError("The directive " + var + " does not exists in this proyect");
}

void	ParserConfig::addRouteVar(std::string const &var, VectorStr values, RouteConfig &route)
{
	if (var == "root")
		route.setRoot(values[0]);
	else if (var == "index")
		route.addIndexFile(values);
	else if (var == "autoindex")
		route.setAutoIndex(values[0]);
	else if (var == "limit_except" || var == "methods" || var == "allowed_methods")
		route.addMethods(values);
	else if (var == "error_page")
		route.addErrorPage(values);
	else if (var == "client_max_body_size")
		route.setMaxSize(values[0]);
	else if (var == "upload_enable")
		route.enableUpload(values[0]);
	else if (var == "upload_dir" || var == "upload_path")
		route.setUploadPath(values[0]);
	else if (var == "return" || var == "redirect")
		route.setReturn(values);
	else if (var == "cgi_pass" || var == "cgi_path" || var == "fastcgi_pass")
		route.setCgiPass(values[0]);
	else if (var == "cgi_extension")
		route.addCgiExtension(values[0]);
	else if (var == "cgi_enable")
		route.setCgiEnable(values[0]);
	else
		sentError("The directive " + var + " in the route does not exists in this proyect");
}

void	ParserConfig::getDataLine(std::string line, std::string &var, VectorStr &values)
{
	std::istringstream	iss(line);
	std::string			val;

	iss >> var;

	if (iss.fail())
		sentError("Unexpected error in istringstream: " + var);

	std::string::size_type	first = line.find(';');
	if (first != std::string::npos)
	{
		std::string::size_type	second = line.rfind(';');
		if (first != second)
			sentError("Syntax error: You have more than one ; -> " + line);
	}

	while (iss >> val)
	{
		if (iss.fail())
			sentError("Unexpected error in istringstream: " + val);

		values.push_back(val);
	}

	if (var == "}" && values.empty())
		return ;
	else if (var == "}")
		sentError("Syntax error: The keys alone please: " + line);

	std::string last = values.back();
	if (var != "location" && last[last.length() - 1] != ';')
		sentError("Synatx error: the variables have to end with ; -> " + line);
	else if (var != "location")
	{
		last.erase(last.length() - 1);
		values.pop_back();
		values.push_back(last);
	}
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

void	ParserConfig::deleteComment(std::string &line)
{
	size_t	pos = line.find('#');

	if (pos != std::string::npos)
		line = line.substr(0, pos);
}

std::vector<ServerConfig> const		&ParserConfig::getServers() const
{
	return _servers;
}

unsigned int	ParserConfig::size() const
{
	return _servers.size();
}
