/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  HttpResponse.hpp                                     :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/05/07 17:02:43 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/07/05 18:14:31 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "HttpRequest.hpp"
# include "ServerConfig.hpp"

class	HttpResponse
{
	public:
		~HttpResponse( void );
		HttpResponse( HttpRequest const &req, ServerConfig const &conf );
		//HttpResponse( HttpResponse const &obj );

		HttpResponse	& operator = ( HttpResponse const &obj );

		std::string	generate( void ) const;

		static std::string	_fileType( std::string const &file );
		static std::string	createError( int code, ServerConfig const &conf );

		std::string			executeGetCgi( std::string const &command, HttpRequest const &request);
		std::string			executePostCgi( std::string const &command, HttpRequest const &request);
		static bool			isCgi( std::string const &command );
		static bool			isCgiAllowed( std::string const &command,
				ServerConfig const &server, Method method );

	private:
		void	getResource( HttpRequest const &request );
		void	postResource( HttpRequest const &request );
		void	deleteResource( HttpRequest const &request );

		void	getCgi( HttpRequest const &request );
		void	postCgi(HttpRequest const &request);

		std::string const	&getPathUpload(std::string const &location) const;
		std::string const	&getCgiPath(std::string const &location) const;

//		static bool			_validFile( std::string const &file );
		static int			checkFile( std::string const &file );
		static std::string	_indexFolder( std::string &folder );

		void		searchGETendPoint( std::string &file );
		void		searchPOSTendPoint( std::string &file );
		void		searchDELETEendPoint( std::string &file );
		//Esta puede cambiar
		static void	getHeaderBody( int code, std::string &body,
				Headers &header, ServerConfig const &config);
//		std::string	_searchEndpoint( std::string const &path );

		std::string			_version;
		int					_code;
		Headers				_header;
		std::string			_body;
		BaseConfig const	*_route;
		ServerConfig const	&_serverConf;
		bool				_index;
};

#endif
