/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:04:46 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/04/26 14:08:50 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "RouteConfig.hpp"
#include <arpa/inet.h>

typedef std::map<in_addr_t, std::vector<in_port_t> >	ListenMap;

class ServerConfig: public BaseConfig
{
	public:
	
		ServerConfig();
		virtual ~ServerConfig();

		void	addListen(std::string listen);
		void	addServerName(std::string name);
		void	addRoute(RouteConfig route);

		ListenMap const					&getListen() const;
		bool 							getListenPorts(std::string ip, std::vector<in_port_t> ports);
		std::vector<std::string> const	&getServerNames() const;
		std::vector<RouteConfig> const	&getRoutes() const;
		
		void	sentError(std::string msg) const;
		void	display();
		void	addDefault();

	private:

		ListenMap					listen;
		std::vector<std::string>	serverNames;
		std::vector<RouteConfig>	routes;

		void	setIpAndPort(std::string listen, std::string &ip, std::string &port);

		in_addr_t	getInet(std::string const &ip);
		in_port_t	getPort(std::string const &port);
		std::string	getIp(in_addr_t inet);

};

#endif
