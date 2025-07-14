/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:42 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/07/14 17:31:09 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/RouteConfig.hpp"

RouteConfig::RouteConfig(): cgiEnable(false) {}
RouteConfig::~RouteConfig() {}

void	RouteConfig::sentError(std::string msg) const
{
	throw std::invalid_argument("<RouteConfig> : " + msg);
}

// cgi_pass/cgi_path [address]

void	RouteConfig::setCgiPath(VectorStr const &data)
{
	if (data.size() != 1)
		sentError("Syntax error: cgi -> cgi_pass [on | off]");
	
	if (cgiPath != "")
		sentError("You cannot have more than one cgi_pass in a route");

	cgiPath = data[0];
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

// upload_path [path]
// default: html/upload

void	RouteConfig::setUploadPath(VectorStr const &data)
{
	if (data.size() != 1)
		sentError("Syntax error: upload_path [path]");
	if (uploadPath != "")
		sentError("You cannot have multiple upload path");
	uploadPath = cleanLine(data[0]);
	if (uploadPath[0] != '/')
		uploadPath = getAbsolute(uploadPath);
		
}

std::string const	&RouteConfig::getCgiPath() const
{
	return cgiPath;
}

VectorStr const	&RouteConfig::getCgiExtensions() const
{
	return cgiExtensions;
}

std::string const	&RouteConfig::getUploadPath() const
{
	return uploadPath;
}

bool	RouteConfig::isCgiEnabled() const
{
	return cgiEnable;
}

// Add all the data that is not in the configuration file

void	RouteConfig::addDefault(BaseConfig const &base)
{
	if (path.empty())
		path = "/";
	if (root.empty())
		this->root = base.getRoot();
	if (errorPages.empty())
		errorPages = base.getErrorPages();
	if (indexFiles.empty())
		indexFiles = base.getIndexFiles();
	if (!clientMaxBodySize)
		clientMaxBodySize = base.getMaxSize();
	if (uploadPath.empty())
	{
		uploadPath = root;
		uploadPath.append("/uploads");
	}
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
	std::cout << "CgiEnable: " << cgiEnable << std::endl;
	std::cout << "CgiPass: " << cgiPath << std::endl;
	
	for (size_t i = 0; i < cgiExtensions.size(); i++)
		std::cout << "CgiExtension: " << cgiExtensions[i] << std::endl;
}
