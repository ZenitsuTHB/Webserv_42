/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*  BaseConfig.cpp                                       :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:08:46 by adrmarqu          #+#    #+#             */
/*  Updated: 2025/06/28 20:00:07 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/BaseConfig.hpp"
#include <cstdlib>
#include <sstream>

BaseConfig::~BaseConfig() {}

BaseConfig::BaseConfig(): autoindex(false), returnCode(-1), clientMaxBodySize(0) {}

void	BaseConfig::sentError(std::string msg) const
{	
	throw std::invalid_argument("<BaseConfig> : " + msg);
}

void	BaseConfig::setPath(VectorStr const &data)
{
	if (data.size() != 1)
		sentError("Syntax error: multiple path -> location [path]");

	path = cleanLine(data[0]);

	if (path[0] != '/')
		sentError("Syntax error: bad start [/] -> location /... -> " + path);
}

// limit_except/methods/allowed_methods [methods(GET, POST, DELETE)]
// default: 000

void	BaseConfig::addMethods(VectorStr const &data)
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

// autoindex [on | off]
// default: off

void	BaseConfig::setAutoindex(VectorStr const &data)
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

void	BaseConfig::setAutoindex(bool autoindex)
{
	this->autoindex = autoindex;
}

// root [URL]
// default: html

void	BaseConfig::setRoot(VectorStr const &data)
{
	if (data.size() != 1)
		sentError("Syntax error: root -> root [path]");

	if (root != "")
		sentError("You cannot have multiple root");

	root = cleanLine(data[0]);
	
	if (root[0] != '/')
		sentError("Syntax error: bad start [/] -> root [/URL] -> " + root);
}

void	BaseConfig::setRoot(std::string const &root)
{
	this->root = cleanLine(root);
	
	if (root[0] != '/')
		sentError("Syntax error: bad start [/] -> root [/URL] -> " + root);
}

// index [URLS]
// default: index.html

void	BaseConfig::addIndexFile(VectorStr const &data)
{
	for (unsigned int i = 0; i < data.size(); i++)
		indexFiles.push_back(data[i]);
}

// error_page [codes] [URL]

void	BaseConfig::addErrorPage(VectorStr const &data)
{
	if (data.size() < 2)
		sentError("Sintax error: error_page -> error_page [codes] [path]");

	for (unsigned int i = 0; i < data.size() - 1; i++)
	{
		for (unsigned int j = 0; j < data[i].length(); j++)
			if (!std::isdigit(data[i][j]))
				sentError("The code has to be a number: " + data[i]);
		
		int	num = std::atoi(data[i].c_str());
		
		if (data[i].size() != 3 || !isValidCode(num))
			sentError("(error_page) : That code does not exists : " + data[i]);
		
		errorPages.insert(std::make_pair(num, data.back()));
	}
}

void	BaseConfig::addErrorPage(ErrorMap const &errors)
{
	this->errorPages = errors;
}

// Check if the code of error/redirection exists

bool	BaseConfig::isValidCode(int num) const
{
	int	codes [] = {
		100, 101, 102, 103,
		200, 201, 202, 204, 205, 206, 207, 208, 214, 226,
		300, 301, 302, 303, 304, 305, 307, 308,
		400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410,
		411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421,
		422, 423, 424, 425, 426, 428, 429, 431, 444, 450, 451,
		495, 496, 497, 498, 499,
		500, 501, 502, 503, 504, 506, 507, 508, 509, 510, 511,
		521, 522, 523, 525, 530, 599, 0};

	for (unsigned int i = 0; codes[i]; i++)
		if (codes[i] == num)
			return true;
	return false;
}

// return (optional)[code] [URL]
// default: [-1] [""]

void	BaseConfig::setReturn(VectorStr const &data)
{
	if (returnCode != -1)
		sentError("You cannot have multiple redirections");

	if (data.size() == 1)
	{
		returnCode = 302;
		redirectUrl = data[0];
	}
	else if (data.size() == 2)
	{
		for (unsigned int i = 0; i < data[0].size(); i++)
			if (!std::isdigit(data[0][i]))
				sentError("The return code has to be a number: " + data[0]);
		
		int	num = std::atoi(data[0].c_str());

		if (data[0].size() != 3 || !isValidCode(num))
			sentError("(return) : That code does not exists : " + data[0]);

		returnCode = num;
		redirectUrl = data[1];
	}
	else
		sentError("Sintax error -> return (optional)[code] [URL]");
}

void	BaseConfig::setReturn(int code, std::string const &url)
{
	returnCode = code;
	redirectUrl = url;
}

// Converts into a bytes (size_t)

size_t	BaseConfig::getBytes(std::string const &val)
{
	char 		c = 'B';
	
	for (unsigned int i = 0; i < val.size(); i++)
	{
		if (!std::isdigit(val[i]))
		{
			std::string	byte = val.substr(i);
			if (byte == "K" || byte == "KB")
				c = 'K';
			else if (byte == "M" || byte == "MB")
				c = 'M';
			else
				sentError("Wrong mesure unity: " + byte);
			break ;
		}
	}

	size_t value = static_cast<size_t>(std::atoi(val.c_str()));

	if (!value)
		sentError("The size ahs to be between 10K and 100M");

	switch (c)
	{
		case 'K':
			value *= 1024;
			break ;
		case 'M':
			value *= 1024 * 1024;
			break ;
	}
	return value;
}

// client_max_body_size [value]
// default: 1M

void	BaseConfig::setMaxSize(VectorStr const &data)
{
	if (data.size() != 1)
		sentError("Syntax error: client size -> client_max_body_size [value]");

	if (clientMaxBodySize != 0)
		sentError("You only need one client max body");

	size_t	val = getBytes(data[0]);
	
	if (val > MAX_SIZE_CLIENT || val < MIN_SIZE_CLIENT)
		sentError("The client max body size is too big (10KB - 100MB)");
	
	clientMaxBodySize = val;
}

void	BaseConfig::setMaxSize(size_t size)
{
	if (clientMaxBodySize != 0)
		sentError("You only need one client max body");
	
	if (size > MAX_SIZE_CLIENT || size < MIN_SIZE_CLIENT)
		sentError("The client max body size is too big (10KB - 100MB)");
	
	clientMaxBodySize = size;
}

bool	BaseConfig::isAutoindex() const
{
	return autoindex;
}

std::string const	&BaseConfig::getRoot() const
{
	return root;
}

VectorStr const	&BaseConfig::getIndexFiles() const
{
	return indexFiles;
}

std::string const	&BaseConfig::getPath() const
{
	return path;
}

std::bitset<SIZE> const	&BaseConfig::getMethods() const
{
	return methods;
}

bool	BaseConfig::isAllowed(Method method) const
{
	if (methods.test(method))
		return true;
	return false;
}

ErrorMap const	&BaseConfig::getErrorPages() const
{
	return errorPages;
}

std::string const	BaseConfig::getErrorPage(int code)
{
	ErrorMap::iterator	it = errorPages.find(code);
	
	std::cerr << "I'm here" << std::endl;
	for (ErrorMap::iterator	it = errorPages.begin();
			it != errorPages.end(); ++it)
	{
		std::cout << it->first << ": " << it->second << std::endl;
	}
	if (it == errorPages.end())
		return "";
	return it->second;
}

int	BaseConfig::getReturnCode() const
{
	return returnCode;
}

std::string const	&BaseConfig::getRedirectUrl() const
{
	return redirectUrl;
}

size_t	BaseConfig::getMaxSize() const
{
	return clientMaxBodySize;
}

std::string	BaseConfig::cleanLine(std::string const &path) const
{
	std::string	newPath;

	for (size_t i = 0; i < path.length(); i++)
	{
		newPath += path[i];
		if (path[i] == '/')
		{
			while (i < path.length() && path[i] == '/')
				i++;
			i--;
		}
	}
	if (newPath.length() > 1 && newPath[newPath.length() - 1] == '/')
		newPath.erase(newPath.length() - 1);
	return newPath;
}
