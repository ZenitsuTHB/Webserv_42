/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:42 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/06 14:25:01 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RouteConfig.hpp"

RouteConfig::RouteConfig(): autoindex(false), uploadEnable(false), maxUploadSize(0), cgiEnable(false) {}
RouteConfig::~RouteConfig() {}

void	RouteConfig::sentError(std::string msg) const
{
	throw std::invalid_argument("<RouteConfig> : " + msg);
}

// location [uri] {}

void	RouteConfig::setPath(std::string const &path)
{
	this->path = path;
}

// autoindex [on | off]
// default: off

void	RouteConfig::setAutoIndex(std::string const &autoindex)
{
	if (autoindex == "on")
		this->autoindex = true;
	else if (autoindex == "off")
		this->autoindex = false;
	else
		sentError("autoindex has to be on or off: " + autoindex);
}

// limit_except/methods/allowed_methods [methods(GET, POST, DELETE)]
// default: 000

void	RouteConfig::addMethods(VectorStr const &values)
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

// fastcgi_pass/cgi_pass/cgi_path [address]

void	RouteConfig::setCgiPass(std::string const &cgi)
{
	cgiPass = cgi;
}

// cgi_extension [extension]

void	RouteConfig::addCgiExtension(std::string const &ext)
{
	if (ext[0] != '.')
		sentError("The extensions have to start with '.' -> " + ext);
	cgiExtensions.push_back(ext);
}

// cgi_enable [on | off]
// default: false

void	RouteConfig::setCgiEnable(std::string const &enable)
{
	if (enable == "on")
		cgiEnable = true;
	else if (enable == "off")
		cgiEnable = false;
	else
		sentError("Syntax error: true | false: " + enable);
}

// max_upload_size [value]
// default: 1M

void	RouteConfig::setMaxUploadSize(std::string const &size)
{
	if (maxUploadSize != 0)
		sentError("You only need one client max body");

	size_t	val = getBytes(size);
	
	if (val > MAX_SIZE || val < MIN_SIZE)
		sentError("The upload max size has to be between (10KB - 100MB)");
	
	maxUploadSize = val;
}

// enable_upload/allow_upload [true | false]
// default: false

void	RouteConfig::enableUpload(std::string const &enable)
{
	if (enable == "on")
		uploadEnable = true;
	else if (enable == "off")
		uploadEnable = false;
	else
		sentError("Enable upload only can be true or false");
}

// upload_dir/upload_path
// default: ./uploads

void	RouteConfig::setUploadPath(std::string const &path)
{
	uploadPath = path;
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

bool	RouteConfig::isAllowedUpload() const
{
	return uploadEnable;
}

std::string const	RouteConfig::getUploadPath() const
{
	return uploadPath;
}

size_t	RouteConfig::getMaxUploadSize() const
{
	return maxUploadSize;
}

void	RouteConfig::addDefault()
{
	
}

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
	std::cout << "UploadEnabled: " << uploadEnable << std::endl;
	std::cout << "UploadMaxBody: " << maxUploadSize << std::endl;
	std::cout << "UploadPath: " << uploadPath << std::endl;
	std::cout << "CgiEnable: " << cgiEnable << std::endl;
	std::cout << "CgiPass: " << cgiPass << std::endl;
	
	for (size_t i = 0; i < cgiExtensions.size(); i++)
		std::cout << "CgiExtension: " << cgiExtensions[i] << std::endl;
}
