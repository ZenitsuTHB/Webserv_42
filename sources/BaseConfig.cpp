/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:08:46 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/10 20:31:25 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/BaseConfig.hpp"
#include <cstdlib>
#include <sstream>

BaseConfig::~BaseConfig() {}

BaseConfig::BaseConfig(): returnCode(-1), clientMaxBodySize(0) {}

void	BaseConfig::sentError(std::string msg) const
{	
	throw std::invalid_argument("<BaseConfig> : " + msg);
}

// root [URL]
// default: html

void	BaseConfig::setRoot(VectorStr const &data)
{
	if (data.size() != 1)
		sentError("Syntax error: root -> root [path]");

	if (root != "")
		sentError("You cannot have multiple root");

	root = data[0];
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

// Check if the code of error/redirection exists

bool	BaseConfig::isValidCode(int num) const
{
	int	codes [] = {200, 301, 302, 303, 304, 307, 308, 400,
					401, 403, 404, 405, 500, 502, 503, 504, 0};

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
	
	if (val > MAX_SIZE || val < MIN_SIZE)
		sentError("The client max body size is too big (10KB - 100MB)");
	
	clientMaxBodySize = val;
}

void	BaseConfig::setMaxSize(size_t size)
{
	if (clientMaxBodySize != 0)
		sentError("You only need one client max body");
	
	if (size > MAX_SIZE || size < MIN_SIZE)
		sentError("The client max body size is too big (10KB - 100MB)");
	
	clientMaxBodySize = size;
}

std::string const	&BaseConfig::getRoot() const
{
	return root;
}

VectorStr const	&BaseConfig::getIndexFiles() const
{
	return indexFiles;
}

ErrorMap const	&BaseConfig::getErrorPages() const
{
	return errorPages;
}

std::string const	BaseConfig::getErrorPage(int code)
{
	ErrorMap::iterator	it = errorPages.find(code);
	
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
