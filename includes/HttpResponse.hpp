/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  HttpResponse.hpp                                     :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/05/07 17:02:43 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/05/12 21:14:37 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "HttpRequest.hpp"

class	HttpResponse
{
	public:
		~HttpResponse( void );
		HttpResponse( HttpRequest const &request );
		HttpResponse( HttpResponse const &obj );

		HttpResponse	& operator = ( HttpResponse const &obj );

		std::string	generate( void ) const;

	private:
		void	getResource( HttpRequest const &request );
		void	postResource( HttpRequest const &request );
		void	deleteResource( HttpRequest const &request );

		static std::string	_fileType( std::string const &file );
		static std::string	_searchEndpoint( std::string const &path );
		static bool			_validFile( std::string const &file );

		std::string	_version;
		int			_code;
		Headers		_header;
		std::string	_body;
};

#endif
