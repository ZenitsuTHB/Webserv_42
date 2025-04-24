/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:42 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/04/24 14:38:22 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RouteConfig.hpp"

RouteConfig::RouteConfig() {}
RouteConfig::~RouteConfig() {}

void	RouteConfig::setPath(std::string path)
{
	(void)path;
}

void	RouteConfig::setRoot(std::string root)
{

	(void)root;
}

void	RouteConfig::addIndex(std::string str)
{

	(void)str;
}

void	RouteConfig::setAutoIndex(bool x)
{

	(void)x;
}

void	RouteConfig::addMethod(HttpMetthod method)
{

	(void)method;
}

void	RouteConfig::setReturn(int code, std::string url)
{

	(void)code;
	(void)url;
}

void	RouteConfig::addCgi(std::string extension, std::string program)
{

	(void)extension;
	(void)program;
}

void	RouteConfig::setUpload(bool enabled, std::string path)
{

	(void)enabled;
	(void)path;
}

void	RouteConfig::setMaxSize(size_t max)
{

	(void)max;
}

std::string const	&RouteConfig::getPath() const
{
	return path;
}

std::string	const	&RouteConfig::getRoot() const
{
	return root;
}

std::vector<std::string> const	&RouteConfig::getIndexVector() const
{
	return	index;
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

int	RouteConfig::getReturnCode() const
{
	return returnCode;
}

std::string	const	&RouteConfig::getReturnUrl() const
{
	return redirectUrl;
}

CgiMap const	&RouteConfig::getCgiHandlers() const
{
	return cgiHandlers;
}

bool	RouteConfig::isUploadEnabled() const
{
	return uploadEnabled;
}

std::string	const	&RouteConfig::getUploadPath() const
{
	return uploadPath;
}

size_t	RouteConfig::getMaxSize() const
{
	return maxBodySize;
}
