/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:51:21 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/04/26 14:09:01 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTECONFIG_HPP
#define ROUTECONFIG_HPP

#include <bitset>
#include "BaseConfig.hpp"

typedef std::map<int, std::string>	ErrorMap;

enum	HttpMetthod
{
	GET = 0,
	POST,
	DELETE,
	SIZE
};

class RouteConfig: public BaseConfig
{
	public:

		RouteConfig();
		virtual ~RouteConfig();

		void	setPath(std::string path);
		void	setAutoIndex(std::string x);
		void	addMethod(std::string limit);
		void	enableUplaod(std::string enable);
		void	setUploadPath(std::string path);
		void	setCgiPass(std::string cgi);
		void	addCgiExtension(std::string ext);
		
		std::string const				&getPath() const;
		bool							isAutoindex() const;
		std::bitset<SIZE> const			&getMethods() const;
		bool							isAllowed(HttpMetthod method) const;
		bool							isUploadEnabled() const;
		std::string	const				&getUploadPath() const;
		std::string const				&getCgiPass() const;
		std::vector<std::string> const	&getCgiExtensions() const;

		void	sentError(std::string msg) const;
		void	display();
		void	addDefault();

	private:

		std::string					path;
		bool						autoindex;
		std::bitset<SIZE>			methods;
		bool						uploadEnabled;
		std::string					uploadPath;
		std::string					cgiPass;
		std::vector<std::string>	cgiExtensions;
};

#endif
