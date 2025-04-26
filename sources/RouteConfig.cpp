/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:42 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/04/26 14:09:20 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RouteConfig.hpp"

RouteConfig::RouteConfig(): autoindex(false), uploadEnabled(false) {}
RouteConfig::~RouteConfig() {}

void	RouteConfig::sentError(std::string msg) const
{
	throw std::invalid_argument("<RouteConfig> : " + msg);
}

void	RouteConfig::setPath(std::string path)
{
	this->path = path;
}

void	RouteConfig::setAutoIndex(std::string x)
{
	if (x == "on")
		autoindex = true;
	else if (x == "off")
		autoindex = false;
	else
		sentError("autoindex has to be on or off: " + x);
}

void	RouteConfig::addMethod(std::string limit)
{
	if (limit == "GET")
		methods.set(GET);
	else if (limit == "POST")
		methods.set(POST);
	else if (limit == "DELETE")
		methods.set(DELETE);
	else
		sentError("The methods are GET, POST, DELETE: " + limit);
}

void	RouteConfig::enableUplaod(std::string enable)
{
	if (enable == "true")
		uploadEnabled = true;
	else if (enable == "false")
		uploadEnabled = false;
	else
		sentError("Sintax error only true or false: " + enable);
}

void	RouteConfig::setUploadPath(std::string path)
{
	uploadPath = path;
}

void	RouteConfig::setCgiPass(std::string cgi)
{
	cgiPass = cgi;
}

void	RouteConfig::addCgiExtension(std::string ext)
{
	if (ext[0] != '.')
		sentError("The extensions have to start with '.' -> " + ext);
	cgiExtensions.push_back(ext);
}

std::string const	&RouteConfig::getPath() const
{
	return path;
}

bool	RouteConfig::isAutoindex() const
{
	return autoindex;
}

std::bitset<SIZE> const	&RouteConfig::getMethods() const
{
	return methods;
}

bool	RouteConfig::isAllowed(HttpMetthod method) const
{
	if (methods.test(method))
		return true;
	return false;
}

bool	RouteConfig::isUploadEnabled() const
{
	return uploadEnabled;
}

std::string	const	&RouteConfig::getUploadPath() const
{
	return uploadPath;
}

std::string const	&RouteConfig::getCgiPass() const
{
	return cgiPass;
}

std::vector<std::string> const	&RouteConfig::getCgiExtensions() const
{
	return cgiExtensions;
}

void	RouteConfig::display()
{
	std::cout << std::endl << "Route: " << path << std::endl << std::endl;

	std::cout << "Root: " << root << std::endl;

	for (size_t i = 0; i < indexFiles.size(); i++)
		std::cout << "IndexFile: " << indexFiles[i] << std::endl;

	for (ErrorMap::iterator it = errorPages.begin(); it != errorPages.end(); it++)
		std::cout << "Error page " << it->first << ": " << it->second << std::endl;
	
	std::cout << "Return code: " << returnCode << std::endl;
	std::cout << "Redirect url: " << redirectUrl << std::endl;
	std::cout << "ClientMaxBody: " << clientMaxBodySize << std::endl;
	std::cout << "Autoindex: " << autoindex << std::endl;
	std::cout << "Methods: " << methods << std::endl;
	std::cout << "UploadEnabled: " << uploadEnabled << std::endl;
	std::cout << "UploadPath: " << uploadPath << std::endl;
	std::cout << "CgiPass: " << cgiPass << std::endl;
	
	for (size_t i = 0; i < cgiExtensions.size(); i++)
		std::cout << "CgiExtension: " << cgiExtensions[i] << std::endl;
}
	
void	RouteConfig::addDefault()
{
	
}
