/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  HttpRequest.hpp                                      :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/04/23 22:39:01 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/05/07 19:32:50 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <string>
# include "Http.hpp"

class	HttpRequest
{
	public:
		~HttpRequest( void );
		HttpRequest( std::string const &req );
		HttpRequest( HttpRequest const &obj );

		void	print( void ) const;

		Method				getMethod( void ) const { return (_method); };
		std::string const	&getPath( void ) const { return (_path); };

		HttpRequest	& operator = ( HttpRequest const &obj );

	private:
		Method		_method;
		std::string	_path;
		std::string	_version;
		Headers		_header;
		std::string _body;
};

#endif
