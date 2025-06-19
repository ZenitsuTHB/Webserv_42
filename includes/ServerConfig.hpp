/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*  ServerConfig.hpp                                     :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:04:46 by adrmarqu          #+#    #+#             */
/*  Updated: 2025/06/17 18:23:16 by mvelazqu           ###   ########.fr      */
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

		void	setListen(VectorStr const &data);
		void	setServerName(VectorStr const &data);
		void	setBacklog(VectorStr const &data);
		void	addRoute(RouteConfig route);

		in_addr_t						getIp() const;
		in_port_t						getPort() const;
		std::string const				&getServerName() const;
		RouteConfig const				*getLocation(std::string const &request) const;
//		RouteConfig const				&getRoute(std::string const &path) const;
		std::vector<RouteConfig> const	&getRoutes() const;
		unsigned int					getNumRoutes() const;
		int								getBacklog() const;
		
		void	sentError(std::string msg) const;
		void	display();
		void	addDefault();

	private:

		std::string					ipStr;
		in_addr_t					ip;
		in_port_t					port;
		int							backlog;
		std::string					serverName;
		std::vector<RouteConfig>	routes;

		in_addr_t	getInet(std::string const &ip);
		in_port_t	getPortNum(std::string const &port);
		std::string	convertIp(in_addr_t inet);
		std::string	lookForHost(std::string const &hostname);

};

#endif
