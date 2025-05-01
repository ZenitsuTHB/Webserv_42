/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:51:21 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/01 18:34:47 by adrmarqu         ###   ########.fr       */
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

		void	setPath(VectorS const &value);
		void	setAutoIndex(VectorS const &value);
		void	addMethods(VectorS const &values);
		void	enableUplaod(VectorS const &value);
		void	setUploadPath(VectorS const &value);
		void	setCgiPass(VectorS const &value);
		void	addCgiExtension(VectorS value);
		
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
