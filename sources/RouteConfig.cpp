/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:42 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/01 18:35:27 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RouteConfig.hpp"

RouteConfig::RouteConfig(): autoindex(false), uploadEnabled(false) {}
RouteConfig::~RouteConfig() {}

void	RouteConfig::sentError(std::string msg) const
{
	throw std::invalid_argument("<RouteConfig> : " + msg);
}

void	RouteConfig::setPath(VectorS const &value)
{
	path = value[0];
}

void	RouteConfig::setAutoIndex(VectorS const &value)
{
	if (value[0] == "on")
		autoindex = true;
	else if (value[0] == "off")
		autoindex = false;
	else
		sentError("autoindex has to be on or off: " + value[0]);
}

void	RouteConfig::addMethods(VectorS const &values)
{
	for (unsigned int i = 0; i < values.size(); i++)
	{
		if (values[i] == "GET")
			methods.set(GET);
		else if (values[i] == "POST")
			methods.set(POST);
		else if (values[i] == "DELETE")
			methods.set(DELETE);
		else
			sentError("The methods are GET, POST, DELETE: " + values[i]);
	}
}

void	RouteConfig::enableUplaod(VectorS const &value)
{
	if (value[0] == "true")
		uploadEnabled = true;
	else if (value[0] == "false")
		uploadEnabled = false;
	else
		sentError("Sintax error only true or false: " + value[0]);
}

void	RouteConfig::setUploadPath(VectorS const &value)
{
	uploadPath = value[0];
}

void	RouteConfig::setCgiPass(VectorS const &value)
{
	cgiPass = value[0];
}

void	RouteConfig::addCgiExtension(VectorS value)
{
	if (value[0][0] != '.')
		sentError("The extensions have to start with '.' -> " + value[0]);
	cgiExtensions.push_back(value[0]);
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

VectorS const	&RouteConfig::getCgiExtensions() const
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
