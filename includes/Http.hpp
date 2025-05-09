/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  Http.hpp                                             :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/04/23 21:16:17 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/05/08 19:26:12 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

# include <string>
# include <map>

# define CRLF "\r\n"
# define CRLF2 "\n"

typedef enum e_method
{
	NO_METHOD = 0,
	GET,
	POST,
	DELETE
}	Method;

typedef std::map< std::string, std::string >	Headers;

class	Http
{
	public:
		static Method		toMethod( std::string const &method );
		static std::string	toString( Method method );
		static std::string	getError( int idx );
		static std::string	getFileType( std::string const key );

		Http	& operator = ( Http const &obj ) { (void)obj; return (*this);};

	private:
		~Http( void ) { return ; };
		Http( void ) { return ; };
		Http( Http const &obj ) { (void)obj; return ; };
		static const std::map< int, std::string >			_errorCode;
		static const std::map< std::string, std::string >	_fileType;
};

#endif
