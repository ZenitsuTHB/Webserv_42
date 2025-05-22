/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:42 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/22 16:58:14 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/RouteConfig.hpp"

RouteConfig::RouteConfig(): autoindex(false), cgiEnable(false) {}
RouteConfig::~RouteConfig() {}

void	RouteConfig::sentError(std::string msg) const
{
	throw std::invalid_argument("<RouteConfig> : " + msg);
}

// location [uri] {}

void	RouteConfig::setPath(VectorStr const &data)
{
	if (data.size() != 1)
		sentError("Syntax error: multiple path -> location [path]");
	
	this->path = data[0];
}

// autoindex [on | off]
// default: off

void	RouteConfig::setAutoIndex(VectorStr const &data)
{
	if (data.size() != 1)
		sentError("Syntax error: autoindex -> autoindex [on | off]");

	if (data[0] == "on")
		autoindex = true;
	else if (data[0] == "off")
		autoindex = false;
	else
		sentError("Syntax error: autoindex -> autoindex [on | off]");
}

// limit_except/methods/allowed_methods [methods(GET, POST, DELETE)]
// default: 000

void	RouteConfig::addMethods(VectorStr const &data)
{
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i] == "GET")
			methods.set(GET);
		else if (data[i] == "POST")
			methods.set(POST);
		else if (data[i] == "DELETE")
			methods.set(DELETE);
		else
			sentError("The methods are GET, POST, DELETE: " + data[i]);
	}
}

// cgi_pass/cgi_path [address]

void	RouteConfig::setCgiPass(VectorStr const &data)
{
	if (data.size() != 1)
		sentError("Syntax error: cgi -> cgi_pass [on | off]");
	
	if (cgiPass != "")
		sentError("You cannot have more than one cgi_pass in a route");

	cgiPass = data[0];
}

// cgi_extension [extension]

void	RouteConfig::addCgiExtension(VectorStr const &data)
{
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i][0] != '.')
			sentError("The extensions have to start with '.' -> " + data[i]);
		cgiExtensions.push_back(data[i]);
	}
}

// cgi_enable [on | off]
// default: false

void	RouteConfig::setCgiEnable(VectorStr const &data)
{
	if (data.size() != 1)
		sentError("Syntax error: cgi -> cgi_enable [on | off]");

	if (data[0] == "on")
		cgiEnable = true;
	else if (data[0] == "off")
		cgiEnable = false;
	else
		sentError("Syntax error: cgi -> cgi_enable [on | off]");
}

// upload_dir/upload_path
// default: ./uploads

void	RouteConfig::setUploadPath(VectorStr const &data)
{
	if (data.size() != 1)
		sentError("Syntax error: upload -> upload_path [path]");

	if (uploadPath != "")
		sentError("You cannot have more than one upload path");

	uploadPath = data[0];
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

std::string const	&RouteConfig::getCgiPass() const
{
	return cgiPass;
}

VectorStr const	&RouteConfig::getCgiExtensions() const
{
	return cgiExtensions;
}

bool	RouteConfig::isCgiEnabled() const
{
	return cgiEnable;
}

std::string const	RouteConfig::getUploadPath() const
{
	return uploadPath;
}

// Add all the data that is not in the configuration file

void	RouteConfig::addDefault(std::string const &root, ErrorMap const &errors, VectorStr const &files, size_t size)
{
	if (path.empty())
		path = "/";
	if (root.empty())
		this->root = root;
	if (errorPages.empty())
		errorPages = errors;
	if (indexFiles.empty())
		indexFiles = files;
	if (!clientMaxBodySize)
		clientMaxBodySize = size;
}

// Display all the data of the route

void	RouteConfig::display()
{
	std::cout << std::endl << "Route: " << path << std::endl << std::endl;

	std::cout << "Root: " << root << std::endl;

	for (size_t i = 0; i < indexFiles.size(); i++)
		std::cout << "IndexFile: " << indexFiles[i] << std::endl;

	if (!errorPages.empty())
		for (ErrorMap::iterator it = errorPages.begin(); it != errorPages.end(); it++)
			std::cout << "Error page " << it->first << ": " << it->second << std::endl;
	
	std::cout << "Return code: " << returnCode << std::endl;
	std::cout << "Redirect url: " << redirectUrl << std::endl;
	std::cout << "ClientMaxBody: " << clientMaxBodySize << std::endl;
	std::cout << "Autoindex: " << autoindex << std::endl;
	std::cout << "Methods: " << methods << std::endl;
	std::cout << "UploadPath: " << uploadPath << std::endl;
	std::cout << "CgiEnable: " << cgiEnable << std::endl;
	std::cout << "CgiPass: " << cgiPass << std::endl;
	
	for (size_t i = 0; i < cgiExtensions.size(); i++)
		std::cout << "CgiExtension: " << cgiExtensions[i] << std::endl;
}
