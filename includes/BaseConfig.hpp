/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:04:23 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/06 14:17:23 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASECONFIG_HPP
#define BASECONFIG_HPP

#include <iostream>
#include <map>
#include <vector>

#define MAX_SIZE 104857600
#define MIN_SIZE 10240

typedef std::map<int, std::string>	ErrorMap;
typedef std::vector<std::string>	VectorStr;

class BaseConfig
{
	public:

		BaseConfig();
		virtual ~BaseConfig();

		void	setRoot(std::string const &root);
		void	addIndexFile(VectorStr const &values);
		void	addErrorPage(VectorStr const &values);
		void	setReturn(VectorStr values);
		void	setMaxSize(std::string const &size);

		std::string const	&getRoot() const;
		VectorStr const		&getIndexFiles() const;
		ErrorMap const		&getErrorPages() const;
		std::string const	getErrorPage(int code);
		int					getReturnCode() const;
		std::string const	&getRedirectUrl() const;
		size_t				getMaxSize() const;
		
		virtual void	sentError(std::string msg) const = 0;
		virtual void	addDefault() = 0;
		virtual void	display() = 0;

	protected:

		std::string		root;
		VectorStr		indexFiles;
		ErrorMap		errorPages;
		int				returnCode;
		std::string		redirectUrl;
		size_t			clientMaxBodySize;
		
		size_t	getBytes(std::string const &val);

	private:

		bool	isValidCode(int num) const;

};

#endif
