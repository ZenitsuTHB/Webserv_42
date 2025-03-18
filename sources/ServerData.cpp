/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 11:42:46 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/03/18 14:55:04 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerData.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

ServerData::~ServerData()
{
}

ServerData::ServerData(char const *path): _end(false)
{
	std::ifstream	file(path);
	if (!file.is_open())
		std::cerr << "Error to open the file: " << path << std::endl;
	
	parserConfig(file);

	file.close();
}

void	ServerData::parserConfig(std::ifstream &file)
{
	std::string	line;
	std::string	varname;
	std::string	data;
	size_t		init;

	while (getline(file, line) && !_end)
	{
		// Si la linea esta vacia ir a la siguiente linea
		if (line.empty())
			continue ;

		// Separar la linea en varname y el resto (data)
		std::istringstream	iss(line);
		iss >> varname;
		std::getline(iss, data);
		
		// Quitar espacios a data

		init = 0;
		while (init < data.size() && std::isspace(data[init]))
			init++;
		data = data.substr(init);

		// Mirar que varname sea server
		
		if (varname == "server{" || (varname == "server" && data == "{"))
			parserServer(file, true);
		else if (varname == "server")
			parserServer(file, false);
		else
		{
			std::cerr << "Sintax error: " << line << std::endl;
			return ;
		}
	}
}

bool	 ServerData::isToken(std::ifstream &file)
{
	std::string line;

	while (getline(file, line) && line.empty()) {}
	
	size_t	init = 0;
	while (init < line.size() && std::isspace(line[init]))
		init++;
	line = line.substr(init);
	if (line == "{")
		return true;
	return false;
}

void	ServerData::parserServer(std::ifstream &file, bool token)
{
	std::string	line;
	std::string	varname;
	std::string	data;
	bool		exitBlock = false;

	// Mirar si tiene token por si es server \n{

	if (!token && !isToken(file))
	{
		std::cerr << "Sintax error: server do not have a {" << std::endl;
		_end = 1;
		return ;
	}

	ServerConfig	server;

	// Conseguir los datos del server
	
	while (getline(file, line) && !_end && !exitBlock)
	{
		// Si la linea esta vacia ir a la siguiente linea
		if (line.empty())
			continue ;

		// Separar la linea en varname y el resto (data)
		std::istringstream	iss(line);
		iss >> varname;
		std::getline(iss, data);
		
		// Quitar espacios a data

		size_t init = 0;
		while (init < data.size() && std::isspace(data[init]))
			init++;
		data = data.substr(init);

		// Una funcion que mire lo que es y si tiene un '}'
		
		switch (whatIsThis(varname, data, exitBlock))
		{
			case ROUTE:
				std::cout << "Es una ruta: " << line << std::endl;
				//parserRoute(file, data, server);
				continue ;
			case VAR:
				//std::cout << "Es una variable: " << varname << " " << data << std::endl;
				parserVar(varname, data, server);
				continue ;
			case TOKEN:
				std::cout << "Es un token: " << line << std::endl;
				break ;
			case ERROR:
				std::cerr << "Sintax error: " << line << std::endl;
				_end = true;
		}
	}
	if (!_end)
		_confi.servers.push_back(server);
}

TypeData	ServerData::whatIsThis(std::string var, std::string &data, bool &exitBlock)
{
	if (var == "location")
		return ROUTE;
	else if (var == "}")
	{
		exitBlock = true;
		return TOKEN;
	}
	else if (!data.empty())
	{
		// Mirar que no haya mas de un ';'
		
		int	count = std::count(data.begin(), data.end(), ';');
		if (count == 0 || count > 1)
			return ERROR;

		// Mirar que acabe en ';'

		size_t	end = data.size() - 1;
		while (end > 0)
		{
			if (data[end] == '}' && !exitBlock)
			{
				exitBlock = true;
				data[end] = ' ';
			}
			else if (data[end] == '}' && exitBlock)
				return ERROR;
			else if (data[end] == ';')
				break ;
			else if (!std::isspace(data[end]))
				return ERROR;
			end--;
		}

		if (data[end] != ';')
			return ERROR;
		return VAR;
	}
	else
		return ERROR;
}

void	ServerData::parserVar(std::string const &var, std::string value, ServerConfig &server)
{
	if (var == "autoindex")
		std::cout << "autoindex: " << value << std::endl;
	else if (var == "allow_file_uploads")
		std::cout << "allow_file_uploads: " << value << std::endl;
	else if (var == "client_max_body_size")
		std::cout << "client_max_body_size: " << value << std::endl;
	else if (var == "return" || var == "redirect")
		std::cout << "return: " << value << std::endl;
	else if (var == "root")
		std::cout << "root: " << value << std::endl;
	else if (var == "index")
		std::cout << "index: " << value << std::endl;
	else if (var == "upload_directory")
		std::cout << "upload_directory: " << value << std::endl;
	else if (var == "allowed_methods" || var == "methods" || var == "limit_except")
		std::cout << "methods: " << value << std::endl;
	else if (var == "fastcgi_pass" || var == "cgi_handlers")
		std::cout << "cgi: " << value << std::endl;
	else if (var == "default_server")
		std::cout << "default_server: " << value << std::endl;
	else if (var == "client_max_body_size")
		std::cout << "client_max_body_size: " << value << std::endl;
	else if (var == "server_name")
		std::cout << "server_name: " << value << std::endl;
	else if (var == "listen")
		std::cout << "listen: " << value << std::endl;
	else if (var == "error_page")
		std::cout << "error_page: " << value << std::endl;
	else
	{
		std::cerr << "Error: we do not have this variable: " << var << std::endl;
		_end = 1;
	}
	server.isDefault = true;
}

ConfigFile	ServerData::getConfi() const
{
	return _confi;		
}
