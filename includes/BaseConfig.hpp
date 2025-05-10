/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:04:23 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/10 19:49:04 by adrmarqu         ###   ########.fr       */
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

		void	setRoot(VectorStr const &data);
		void	addIndexFile(VectorStr const &data);
		void	addErrorPage(VectorStr const &data);
		void	setReturn(VectorStr const &data);
		void	setMaxSize(VectorStr const &data);
		void	setMaxSize(size_t size);

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
		
	private:

		bool	isValidCode(int num) const;
		size_t	getBytes(std::string const &val);

};

#endif
