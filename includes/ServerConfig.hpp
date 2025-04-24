/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:04:46 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/04/24 14:30:42 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <iostream>
#include <map>
#include <vector>

typedef std::map<std::string, std::vector<int> >	ListenMap;
typedef std::map<int, std::string>	ErrorMap;

class RouteConfig;

class ServerConfig
{
	public:
	
		ServerConfig();
		~ServerConfig();

		void	addListen(std::string ip, int port);
		void	addServerName(std::string name);
		void	setRoot(std::string root);
		void	addIndexFile(std::string file);
		void	addErrorPage(int code, std::string url);
		void	setMaxSize(size_t max);
		void	setReturn(int code, std::string url);

		ListenMap const					&getListen() const;
		bool 							getListenPorts(std::string ip, std::vector<int> ports);
		std::vector<std::string> const	&getServerNames() const;
		std::string	const				&getRoot() const;
		std::vector<std::string> const	&getIndexFiles() const;
		ErrorMap const					&getErrorPages() const;
		std::string const				getErrorPage(int code);
		size_t							getMaxSize() const;
		int								getReturnCode() const;
		std::string const				&getReturnUrl() const;

	private:

		ListenMap					listen;
		std::vector<std::string>	serverNames;
		std::string					root;
		std::vector<std::string>	indexFiles;
		ErrorMap					errorPages;
		size_t						clientMaxBodySize;
		int							returnCode;
		std::string					redirectUrl;
		std::vector<RouteConfig>	routes;
};

//std::map<std::string IP, ListenSockData data>	listen;

#endif
