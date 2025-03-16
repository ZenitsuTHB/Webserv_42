/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 11:42:46 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/03/16 14:59:35 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerData.hpp"
#include <fstream>

static void	printError(std::string msg, std::string arg)
{
	std::cerr << msg << ": " << arg << std::endl;
}

ServerData::~ServerData()
{
}

ServerData::ServerData(char const *path)
{
	LineType		server;
	std::ifstream	file(path);

	if (!file.is_open())
	{
		std::cerr << "Error to open the file: " << path << std::endl;
		return ;
	}

	std::string	line;
	while (std::getline(file, line))
	{
		server = isServer(line);
		switch (server)
		{
			case EMPTY:
				continue ;
			case SERVER:
				//getServerData(file);
			case ERROR:
				return ;
		}
	}

	file.close();
}
/*
void	ServerData::getServerData(std::ifstream file)
{
	LineType		type;
	ServerConfig	server;
	std::string		line;

	while (std::getline(file, line))
	{
		type = isLocation(line);
		switch (type)
		{
			case EMPTY:
				continue ;
			case VAR:
				// getVarData(line);
			case LOCATION:
				// getLocationData(file, line);
				continue ;
			case END:
				break ;
			case ERROR:
				return ;
		}
	}
	_confi.servers.push_back(server);
}

//void	ServerData::getLocationData(std::ifstream file, std::string line)
//{
	
//}

LineType	ServerData::isLocation(std::string line)
{
	if (line.empty())
		return EMPTY;

	size_t	start = line.find_first_not_of(" \t");
	size_t	end = line.find_last_not_of(" \t");

	std::string	newLine = line.substr(start, end - start + 1);

	if (newLine == "}")
		return END;

	if (newLine.compare(0, 8, "location") != 0)
	{
		if (newLine.find('{') == std::string::npos)
			return VAR;
		else
			return printError("Sintax error", line), ERROR;
	}

	size_t	x = 8;
	while (x < newLine.size() && std::isspace(newLine[x]))
		x++;

	if (newLine.size() == x || newLine[x] == '{')
		return printError("Sintax error", line), ERROR;

	bool key = false;
	for (size_t i = x; i < newLine.size(); i++)
	{
		if (std::isspace(newLine[i]))
			continue ;
		if (!key && newLine[i] == '{')
			key = true;
		else
			return printError("Sintax error", line), ERROR;
	}
	if (!key)
		return printError("Sintax error", line), ERROR;
	return LOCATION;
}*/

LineType	ServerData::isServer(std::string line)
{
	if (line.empty())
		return EMPTY;

	size_t	start = line.find_first_not_of(" \t");
	size_t	end = line.find_last_not_of(" \t");

	std::string	newLine = line.substr(start, end - start + 1);

	if (newLine.compare(0, 6, "server") != 0)
		return printError("Sintax error", line), ERROR;

	bool key = false;
	for (size_t i = 6; i < newLine.size(); i++)
	{
		if (std::isspace(newLine[i]))
			continue ;
		if (!key && newLine[i] == '{')
			key = true;
		else
			return printError("Sintax error", line), ERROR;
	}
	if (!key)
		return printError("Sintax error", line), ERROR;
	return SERVER;
}
