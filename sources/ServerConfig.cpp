/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:23 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/04/24 14:31:06 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RouteConfig.hpp"
#include "../includes/ServerConfig.hpp"
	
ServerConfig::ServerConfig() {}
ServerConfig::~ServerConfig() {}

void	ServerConfig::addListen(std::string ip, int port)
{
	(void)ip;
	(void)port;
}

void	ServerConfig::addServerName(std::string name)
{

	(void)name;
}

void	ServerConfig::setRoot(std::string root)
{

	(void)root;
}

void	ServerConfig::addIndexFile(std::string file)
{
	(void)file;

}

void	ServerConfig::addErrorPage(int code, std::string url)
{

	(void)code;
	(void)url;
}

void	ServerConfig::setMaxSize(size_t max)
{

	(void)max;
}

void	ServerConfig::setReturn(int code, std::string url)
{
	
	(void)code;
	(void)url;

}

ListenMap const	&ServerConfig::getListen() const
{
	return listen;
}

bool	ServerConfig::getListenPorts(std::string ip, std::vector<int> ports)
{
	ListenMap::iterator	it = listen.find(ip);
	if (it == listen.end())
		return false;

	ports = it->second;

	return true;
}

std::vector<std::string> const	&ServerConfig::getServerNames() const
{
	return serverNames;
}

std::string	const	&ServerConfig::getRoot() const
{
	return root;
}

std::vector<std::string> const	&ServerConfig::getIndexFiles() const
{
	return indexFiles;
}

ErrorMap const	&ServerConfig::getErrorPages() const
{
	return errorPages;
}

std::string const	ServerConfig::getErrorPage(int code)
{
	ErrorMap::iterator	it = errorPages.find(code);
	
	if (it == errorPages.end())
		return "";
	return it->second;
}

size_t	ServerConfig::getMaxSize() const
{
	return clientMaxBodySize;
}

int	ServerConfig::getReturnCode() const
{
	return returnCode;
}

std::string	const &ServerConfig::getReturnUrl() const
{
	return redirectUrl;
}

