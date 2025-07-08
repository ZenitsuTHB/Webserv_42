/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:04:23 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/07/06 18:54:35 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASECONFIG_HPP
#define BASECONFIG_HPP

#include <iostream>
#include <map>
#include <vector>
#include <bitset>
#include "Http.hpp"

#define MAX_SIZE_CLIENT 104857600
#define MIN_SIZE_CLIENT 10240

typedef std::map<int, std::string>	ErrorMap;
typedef std::vector<std::string>	VectorStr;
typedef std::bitset<SIZE>			BitMet;

class BaseConfig
{
	public:

		BaseConfig();
		virtual ~BaseConfig();

		void	setPath(VectorStr const &data);
		void	addMethods(VectorStr const &data);
		void	setAutoindex(VectorStr const &data);
		void	setAutoindex(bool autoindex);
		void	setRoot(VectorStr const &data);
		void	setRoot(std::string const &root);
		void	addIndexFile(VectorStr const &data);
		void	addErrorPage(VectorStr const &data);
		void	addErrorPage(ErrorMap const &errors);
		void	setReturn(VectorStr const &data);
		void	setReturn(int code, std::string const &url);
		void	setMaxSize(VectorStr const &data);
		void	setMaxSize(size_t size);

		bool				isAutoindex() const;
		std::string const	&getPath() const;
		BitMet const		&getMethods() const;
		bool				isAllowed(Method method) const;
		std::string const	&getRoot() const;
		VectorStr const		&getIndexFiles() const;
		ErrorMap const		&getErrorPages() const;
		std::string const	getErrorPage(int code) const;
		int					getReturnCode() const;
		std::string const	&getRedirectUrl() const;
		size_t				getMaxSize() const;
		
		virtual void	sentError(std::string msg) const;

	protected:

		bool			autoindex;
		std::string		path;
		BitMet			methods;
		std::string		root;
		VectorStr		indexFiles;
		ErrorMap		errorPages;
		int				returnCode;
		std::string		redirectUrl;
		size_t			clientMaxBodySize;

		std::string		cleanLine(std::string const &path) const;
		
	private:

		std::string	getAbsolute(std::string const &dir);
		bool		isValidCode(int num) const;
		size_t		getBytes(std::string const &val);

};

#endif

/*


autoindex: 					server, location
root:						server, location
index						server, location
error_page					server, location
return						server, location
client_max_body_size		server, location

limit_except/methods		location
cgi_enable					location
cgi_path					location					
cgi_extension				location

listen						server
server_name					server
backlog						server

*/
