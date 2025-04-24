/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:51:21 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/04/24 14:37:26 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTECONFIG_HPP
#define ROUTECONFIG_HPP

#include <iostream>
#include <bitset>
#include <map>
#include <vector>

typedef std::map<std::string, std::string>	CgiMap;

enum	HttpMetthod
{
	GET = 0,
	POST,
	DELETE,
	SIZE
};

class RouteConfig
{
	public:

		RouteConfig();
		~RouteConfig();

		void	setPath(std::string path);
		void	setRoot(std::string root);
		void	addIndex(std::string str);
		void	setAutoIndex(bool x);
		void	addMethod(HttpMetthod method);
		void	setReturn(int code, std::string url);
		void	addCgi(std::string extension, std::string program);
		void	setUpload(bool enabled, std::string path);
		void	setMaxSize(size_t max);

		std::string const				&getPath() const;
		std::string	const				&getRoot() const;
		std::vector<std::string> const	&getIndexVector() const;
		bool							isAutoindex() const;
		std::bitset<SIZE> const			&getMethods() const;
		bool							isAllowed(HttpMetthod method) const;
		int								getReturnCode() const;
		std::string	const				&getReturnUrl() const;
		CgiMap const					&getCgiHandlers() const;
		bool							isUploadEnabled() const;
		std::string	const				&getUploadPath() const;
		size_t							getMaxSize() const;

	private:

		std::string					path;
		std::string					root;
		std::vector<std::string>	index;
		bool						autoindex;
		std::bitset<SIZE>			methods;
		int							returnCode;
		std::string					redirectUrl;
		CgiMap						cgiHandlers;
		bool						uploadEnabled;
		std::string					uploadPath;
		size_t						maxBodySize;
};

#endif
