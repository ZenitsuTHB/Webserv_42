/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:08:46 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/04/26 14:05:52 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/BaseConfig.hpp"
#include <cstdlib>

#define MAX_SIZE_CLIENT 104857600
#define MIN_SIZE_CLIENT 10240

BaseConfig::~BaseConfig() {}

BaseConfig::BaseConfig(): returnCode(-1), clientMaxBodySize(0) {}

void	BaseConfig::sentError(std::string msg) const
{	
	throw std::invalid_argument("<BaseConfig> : " + msg);
}

void	BaseConfig::setRoot(std::string root)
{
	if (this->root != "")
		sentError("You only can have one root");
	this->root = root;
}
		
void	BaseConfig::addIndexFile(std::string file)
{
	indexFiles.push_back(file);
}
	
void	BaseConfig::addErrorPage(std::string code, std::string url)
{
	for (unsigned int i = 0; i < code.size(); i++)
		if (!std::isdigit(code[i]))
			sentError("The code has to be a number: " + code);
	
	int	num = std::atoi(code.c_str());

	if (code.size() != 3 || num < 100 || num >= 600)
		sentError("The code is between 100 and 599");

	errorPages.insert(std::make_pair(num, url));
}

void	BaseConfig::setReturn(std::string code, std::string url)
{
	for (unsigned int i = 0; i < code.size(); i++)
		if (!std::isdigit(code[i]))
			sentError("The return code has to be a number: " + code);
	
	int	num = std::atoi(code.c_str());

	if (code.size() != 3 || num < 100 || num >= 600)
		sentError("The code is between 100 and 599");

	returnCode = num;
	redirectUrl = url;
}

void	BaseConfig::setMaxSize(std::string max)
{
	std::string	num;
	char 		c = 'B';
	size_t		maxi = 10240;

	if (clientMaxBodySize != 0)
		sentError("You only need one client max body");

	for (unsigned int i = 0; i < max.size(); i++)
	{
		if (!std::isdigit(max[i]))
		{
			std::string	byte = max.substr(i);
			if (byte == "K" || byte == "KB")
				c = 'K';
			else if (byte == "M" || byte == "MB")
				c = 'M';
			else
				sentError("Wrong mesure unity: " + byte);
			break ;
		}
	}

	maxi = static_cast<size_t>(std::atoi(max.c_str()));

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

std::vector<std::string> const	&BaseConfig::getIndexFiles() const
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
