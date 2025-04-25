/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:04:46 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/04/25 20:18:52 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "RouteConfig.hpp"

typedef std::map<std::string, std::vector<int> >	ListenMap;

class ServerConfig: public BaseConfig
{
	public:
	
		ServerConfig();
		virtual ~ServerConfig();

		void	addListen(std::string ip, std::string port);
		void	addServerName(std::string name);
		void	addRoute(RouteConfig route);

		ListenMap const					&getListen() const;
		bool 							getListenPorts(std::string ip, std::vector<int> ports);
		std::vector<std::string> const	&getServerNames() const;
		std::vector<RouteConfig> const	&getRoutes() const;
		
		void	sentError(std::string msg) const;
		void	display();

	private:

		ListenMap					listen;
		std::vector<std::string>	serverNames;
		std::vector<RouteConfig>	routes;

};

#endif
