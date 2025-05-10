/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:04:46 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/10 17:49:35 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "RouteConfig.hpp"
#include <arpa/inet.h>

class ServerConfig: public BaseConfig
{
	public:
	
		ServerConfig();
		virtual ~ServerConfig();

		void	setListen(std::string const &listen);
		void	setServerName(std::string const &name);
		void	addRoute(RouteConfig route);

		std::string const				&getIp() const;
		in_addr_t						getIpNum() const;
		in_port_t						getPortNum() const;
		std::string const				&getServerName() const;
		RouteConfig const				&getRoute(int route) const;
		std::vector<RouteConfig> const	&getRoutes() const;
		unsigned int					getNumRoutes() const;
		
		void	sentError(std::string msg) const;
		void	display();
		void	addDefault();

	private:

		std::string					ip;
		in_addr_t					ipNum;
		in_port_t					port;
		int							backlog;
		std::string					serverName;
		std::vector<RouteConfig>	routes;

		in_addr_t	getInet(std::string const &ip);
		in_port_t	getPort(std::string const &port);
		std::string	convertIp(in_addr_t inet);
		std::string	lookForHost(std::string const &hostname);

};

#endif
