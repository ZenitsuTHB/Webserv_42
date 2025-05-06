/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:08:46 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/06 14:15:57 by adrmarqu         ###   ########.fr       */
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

void	BaseConfig::setRoot(std::string const &root)
{
	if (this->root != "")
		sentError("You only can have one root");
	this->root = root;
}

// index [URLS]
// default: index.html

void	BaseConfig::addIndexFile(VectorStr const &values)
{
	for (unsigned int i = 0; i < values.size(); i++)
		indexFiles.push_back(values[i]);
}

// error_page [codes] [URL]

void	BaseConfig::addErrorPage(VectorStr const &values)
{
	if (values.size() < 2)
		sentError("Sintax error (error_page)");

	for (unsigned int i = 0; i < values.size() - 1; i++)
	{
		for (unsigned int j = 0; j < values[i].length(); j++)
			if (!std::isdigit(values[i][j]))
				sentError("The code has to be a number: " + values[i]);
		
		int	num = std::atoi(values[i].c_str());

		if (values[i].size() != 3 || num < 100 || num >= 600)
			sentError("The code is between 100 and 599");
		
		errorPages.insert(std::make_pair(num, values.back()));
	}
}

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

void	BaseConfig::setReturn(VectorStr values)
{
	if (values.size() == 1)
	{
		returnCode = 302;
		redirectUrl = values[0];
	}
	else if (values.size() == 2)
	{
		for (unsigned int i = 0; i < values[0].size(); i++)
			if (!std::isdigit(values[0][i]))
				sentError("The return code has to be a number: " + values[0]);
		
		int	num = std::atoi(values[0].c_str());

		if (values[0].size() != 3 || !isValidCode(num))
			sentError("(return) : That code does not exists : " + values[0]);

		returnCode = num;
		redirectUrl = values[1];
	}
	else
		sentError("Sintax error -> return (optional)[code] [URL]");
}

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

void	BaseConfig::setMaxSize(std::string const &size)
{
	if (clientMaxBodySize != 0)
		sentError("You only need one client max body");

	size_t	val = getBytes(size);
	
	if (val > MAX_SIZE || val < MIN_SIZE)
		sentError("The client max body size is too big (10KB - 100MB)");
	
	clientMaxBodySize = val;
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
