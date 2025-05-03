/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:51:21 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/03 13:47:09 by adrmarqu         ###   ########.fr       */
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
		void	addMethods(VectorS const &values);
		void	enableUplaod(std::string const &enable);
		void	setUploadPath(std::string const &path);
		void	setCgiPass(std::string const &cgi);
		void	addCgiExtension(std::string const &ext);
		
		std::string const			&getPath() const;
		bool						isAutoindex() const;
		std::bitset<SIZE> const		&getMethods() const;
		bool						isAllowed(HttpMetthod method) const;
		bool						isUploadEnabled() const;
		std::string	const			&getUploadPath() const;
		std::string const			&getCgiPass() const;
		VectorS const				&getCgiExtensions() const;

		void	sentError(std::string msg) const;
		void	display();
		void	addDefault();

	private:

		std::string			path;
		bool				autoindex;
		std::bitset<SIZE>	methods;
		bool				uploadEnabled;
		std::string			uploadPath;
		std::string			cgiPass;
		VectorS				cgiExtensions;
};

#endif
