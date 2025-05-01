/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:04:23 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/01 18:18:29 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASECONFIG_HPP
#define BASECONFIG_HPP

#include <iostream>
#include <map>
#include <vector>

typedef std::map<int, std::string>	ErrorMap;
typedef std::vector<std::string>	VectorS;

class BaseConfig
{
	public:

		BaseConfig();
		virtual ~BaseConfig();

		void	setRoot(VectorS const &value);
		void	addIndexFile(VectorS const &values);
		void	addErrorPage(VectorS const &values);
		void	setReturn(VectorS values);
		void	setMaxSize(VectorS const &value);

		std::string const	&getRoot() const;
		VectorS const		&getIndexFiles() const;
		ErrorMap const		&getErrorPages() const;
		std::string const	getErrorPage(int code);
		int					getReturnCode() const;
		std::string const	&getRedirectUrl() const;
		size_t				getMaxSize() const;
		
		virtual void	sentError(std::string msg) const = 0;
		virtual void	display() = 0;
		virtual void	addDefault() = 0;

	protected:

		std::string		root;
		VectorS			indexFiles;
		ErrorMap		errorPages;
		int				returnCode;
		std::string		redirectUrl;
		size_t			clientMaxBodySize;

};

#endif
