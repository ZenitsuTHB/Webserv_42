/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  exceptions.hpp                                       :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/05/06 18:43:16 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/05/06 19:57:24 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

#include <exception>

class	HttpException: public std::exception
{
	public:
		HttpException( std::string const message, int code ):
			_msg(message), _code(code) { return ; }
		~HttpException ( void ) throw() { return ; }

		char const	*what( void ) const throw() { return (_msg.c_str()); }
		int			whatCode( void ) const { return (_code); }
	private:
		std::string	_msg;
		int			_code;
};

#endif
