/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 11:42:46 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/03/20 14:44:11 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerData.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

ServerData::~ServerData()
{
}

ServerData::ServerData(char const *path): _error(false)
{
	std::ifstream	file(path);

	if (!file.is_open())
	{
		std::cerr << "Error to open the file: " << path << std::endl;
		_error = true;
		return ;
	}

	setDataFile(file);
	file.close();
/*
	for (int i = 0; i < _serverData.size(); i++)
		std::cout << _serverData[i] << std::endl;
	std::cout << std::endl;
*/
	parserData();
}

void	ServerData::setDataFile(std::ifstream &file)
{
	std::string	line;
	std::string	data;

	data = "";
	while (getline(file, line))
		data += line;

	std::string			element;
	std::istringstream	iss(data);

	while (iss >> element)
		_serverData.push_back(element);
}

void	ServerData::parserData()
{
	for (int idx = 0; idx < static_cast<int>(_serverData.size()) && !_error; idx++)
	{
		if (_serverData[idx] == "server" && _serverData[idx + 1] == "{")
			parserServer(idx);
		else
		{
			std::cerr << "Sintax error: " << _serverData[idx] << " " << _serverData[idx] << std::endl;
			_error = 1;
			return ;
		}
	}
}

void	ServerData::parserServer(int &idx)
{
	ServerConfig	server;

	idx += 2;

	for (; idx < static_cast<int>(_serverData.size()); idx++)
	{
		if (_serverData[idx] == "location")
			parserRoute(idx, server);
		else if (_serverData[idx] == "}")
			break ;
		else
		{
			std::cout << "Var: " << _serverData[idx] << std::endl;
			//if (parserVar(idx))
			//	return ;
		}
		if (_error)
			return ;
	}
	_servers.push_back(server);
}

void	ServerData::parserRoute(int &idx, ServerConfig &server)
{
	RouteConfig	route;
	
	route.path = _serverData[++idx];
	if  (_serverData[++idx] != "{")
	{
		_error = 1;
		std::cerr << "Sintax error: <location path {>" << std::endl;
		return ;
	}
	idx++;
	for (; idx < static_cast<int>(_serverData.size()); idx++)
	{
		if (_serverData[idx] == "}" || parserRouteVar(idx, route))
			break ;
		if (_error)
			return ;
	}
	server.routes.push_back(route);
}

bool	ServerData::parserRouteVar(int &idx, RouteConfig &route)
{
	//std::string	options[] = {"allowed_methods", "root", "directory_listing", "default_file",
	//						"redirect", "cgi_enabled", "cgi_path", "cgi_extension",
	//						"upload_enable", "upload_store", "autoindex"};
	//bool	(*functions[])(idx, route) = {methods};

	std::cout << "Var de ruta: " << _serverData[idx] << std::endl;
	if (_serverData[idx] == "allowed_methods")
		methods(idx, route);
	//for (int i = 0; i < 11; i++)
	//{
		//if (_serverData[idx] == options[i])
		//	return functions[i](idx, route);
	//}
	//_error = 1;
	//std::cerr << "Error: This variable do not exists in this proyect: ";
	//std::cerr << _serverData[idx] << std::endl;
	return false;
}

bool	ServerData::methods(int &idx, RouteConfig &route)
{
	std::cout << "Metod: " << _serverData[idx] << std::endl;
	(void)route;
	return false;
}
