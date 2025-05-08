/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:51:21 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/08 14:11:22 by adrmarqu         ###   ########.fr       */
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

		void	setPath(std::string const &path);
		void	setAutoIndex(std::string const &autoindex);
		void	addMethods(VectorStr const &values);
		void	setCgiPass(std::string const &cgi);
		void	addCgiExtension(std::string const &ext);
		void	setCgiEnable(std::string const &enable);
		void	enableUpload(std::string const &enable);
		void	setUploadPath(std::string const &path);
		
		std::string const			&getPath() const;
		bool						isAutoindex() const;
		std::bitset<SIZE> const		&getMethods() const;
		bool						isAllowed(HttpMetthod method) const;
		bool						isAllowedUpload() const;
		std::string const			getUploadPath() const;
		bool						isCgiEnabled() const;
		std::string const			&getCgiPass() const;
		VectorStr const				&getCgiExtensions() const;

		void	sentError(std::string msg) const;
		void	addDefault();
		void	display();

	private:

		std::string			path;
		bool				autoindex;
		std::bitset<SIZE>	methods;
		bool				uploadEnable;
		std::string			uploadPath;
		bool				cgiEnable;
		std::string			cgiPass;
		VectorStr			cgiExtensions;
};

#endif
