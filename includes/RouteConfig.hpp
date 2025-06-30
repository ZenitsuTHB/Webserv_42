/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:51:21 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/06/30 18:22:16 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTECONFIG_HPP
#define ROUTECONFIG_HPP

#include <bitset>
#include "BaseConfig.hpp"
#include "Http.hpp"

typedef std::map<int, std::string>	ErrorMap;

class RouteConfig: public BaseConfig
{
	public:

		RouteConfig();
		virtual ~RouteConfig();

		void	setPath(VectorStr const &data);
		void	addMethods(VectorStr const &data);
		void	setCgiPath(VectorStr const &data);
		void	addCgiExtension(VectorStr const &data);
		void	setCgiEnable(VectorStr const &data);
		
		std::string const			&getPath() const;
		std::bitset<SIZE> const		&getMethods() const;
		bool						isAllowed(Method method) const;
		std::string const			getUploadPath() const;
		bool						isCgiEnabled() const;
		std::string const			&getCgiPath() const;
		VectorStr const				&getCgiExtensions() const;

		void	sentError(std::string msg) const;
		void	display();
		void	addDefault(BaseConfig const &base);

	private:

		std::string			path;
		std::bitset<SIZE>	methods;
		bool				cgiEnable;
		std::string			cgiPath;
		VectorStr			cgiExtensions;
};

#endif
