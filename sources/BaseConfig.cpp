/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:08:46 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/03 14:04:08 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/BaseConfig.hpp"
#include <cstdlib>
#include <sstream>

#define MAX_SIZE_CLIENT 104857600
#define MIN_SIZE_CLIENT 10240

BaseConfig::~BaseConfig() {}

BaseConfig::BaseConfig(): returnCode(-1), clientMaxBodySize(0) {}

void	BaseConfig::sentError(std::string msg) const
{	
	throw std::invalid_argument("<BaseConfig> : " + msg);
}

void	BaseConfig::setRoot(std::string const &root)
{
	if (this->root != "")
		sentError("You only can have one root");
	this->root = root;
}
		
void	BaseConfig::addIndexFile(VectorS const &values)
{
	for (unsigned int i = 0; i < values.size(); i++)
		indexFiles.push_back(values[i]);
}
	
void	BaseConfig::addErrorPage(VectorS const &values)
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

void	BaseConfig::setReturn(VectorS values)
{
	if (values.size() != 2)
		sentError("Sintax error (return)");

	for (unsigned int i = 0; i < values[0].size(); i++)
		if (!std::isdigit(values[0][i]))
			sentError("The return code has to be a number: " + values[0]);
	
	int	num = std::atoi(values[0].c_str());

	if (values[0].size() != 3 || num < 100 || num >= 600)
		sentError("The code is between 100 and 599");

	returnCode = num;
	redirectUrl = values[1];
}

void	BaseConfig::setMaxSize(std::string const &size)
{
	std::string	num;
	char 		c = 'B';
	size_t		maxi = 10240;

	if (clientMaxBodySize != 0)
		sentError("You only need one client max body");

	for (unsigned int i = 0; i < size.size(); i++)
	{
		if (!std::isdigit(size[i]))
		{
			std::string	byte = size.substr(i);
			if (byte == "K" || byte == "KB")
				c = 'K';
			else if (byte == "M" || byte == "MB")
				c = 'M';
			else
				sentError("Wrong mesure unity: " + byte);
			break ;
		}
	}

	maxi = static_cast<size_t>(std::atoi(size.c_str()));

	switch (c)
	{
		case 'K':
			maxi *= 1024;
			break ;
		case 'M':
			maxi *= 1024 * 1024;
			break ;
		
	}

	if (maxi > MAX_SIZE_CLIENT || maxi < MIN_SIZE_CLIENT)
		sentError("The client max body size is too big (10KB - 100MB)");
	
	clientMaxBodySize = maxi;
}

std::string const	&BaseConfig::getRoot() const
{
	return root;
}

VectorS const	&BaseConfig::getIndexFiles() const
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
