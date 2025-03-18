/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 11:42:46 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/03/18 13:15:19 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerData.hpp"
#include <fstream>
#include <sstream>

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

	// Buscar un server
	// Buscar un {
	// Guardar datos del server o encontrar un location
	// Buscar un { si encuentras location
	// Guardar datos de location
	// Parar si encuentras un }
	// Buscar otro server si encuentras un }

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
				//parserRoute(file, data);
				continue ;
			case VAR:
				std::cout << "Es una variable: " << line << std::endl;
				//parserVar(varname, data);
				continue ;
			case TOKEN:
				std::cout << "Es un token: " << line << std::endl;
				break ;
			case ERROR:
				std::cerr << "Sintax error: " << line << std::endl;
				_end = true;
		}
	}
}

TypeData	ServerData::whatIsThis(std::string var, std::string data, bool &exitBlock)
{
	if (var == "location")
		return ROUTE;
	else if (var == "}")
	{
		exitBlock = true;
		return TOKEN;
	}
	else if (!data.empty())
		return VAR;
	else
		return ERROR;
	
}

ConfigFile	ServerData::getConfi() const
{
	return _confi;		
}
