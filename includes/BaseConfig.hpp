/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:04:23 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/04/26 14:08:25 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASECONFIG_HPP
#define BASECONFIG_HPP

#include <iostream>
#include <map>
#include <vector>

typedef std::map<int, std::string>	ErrorMap;

class BaseConfig
{
	public:

		BaseConfig();
		virtual ~BaseConfig();

		void	setRoot(std::string root);
		void	addIndexFile(std::string file);
		void	addErrorPage(std::string code, std::string url);
		void	setReturn(std::string code, std::string url);
		void	setMaxSize(std::string max);

		std::string const				&getRoot() const;
		std::vector<std::string> const	&getIndexFiles() const;
		ErrorMap const					&getErrorPages() const;
		std::string const				getErrorPage(int code);
		int								getReturnCode() const;
		std::string const				&getRedirectUrl() const;
		size_t							getMaxSize() const;
		
		virtual void	sentError(std::string msg) const = 0;
		virtual void	display() = 0;
		virtual void	addDefault() = 0;

	protected:

		std::string					root;
		std::vector<std::string>	indexFiles;
		ErrorMap					errorPages;
		int							returnCode;
		std::string					redirectUrl;
		size_t						clientMaxBodySize;

};

#endif
