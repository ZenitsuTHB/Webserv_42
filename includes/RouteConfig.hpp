/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:51:21 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/07/12 13:29:46 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTECONFIG_HPP
#define ROUTECONFIG_HPP

#include "BaseConfig.hpp"

class RouteConfig: public BaseConfig
{
	public:

		RouteConfig();
		virtual ~RouteConfig();

		void	setCgiPath(VectorStr const &data);
		void	addCgiExtension(VectorStr const &data);
		void	setCgiEnable(VectorStr const &data);
		void	setUploadPath(VectorStr const &data);
		
		bool						isCgiEnabled() const;
		std::string const			&getCgiPath() const;
		VectorStr const				&getCgiExtensions() const;
		std::string const			&getUploadPath() const;

		void	sentError(std::string msg) const;
		void	display();
		void	addDefault(BaseConfig const &base);

	private:

		bool				cgiEnable;
		std::string			cgiPath;
		std::string			uploadPath;
		VectorStr			cgiExtensions;
};

#endif
