/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  Http.hpp                                             :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/04/23 21:16:17 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/04/23 22:39:21 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

typedef enum e_method
{
	NO_METHOD = 0,
	GET,
	POST,
	DELETE
}	Method;

class	Http
{
	public:
		static Method		toMethod( std::string const &method );
		static std::string	toStr( Method method );

		Http	& operator = ( Http const &obj ) { (void)obj; };

	private:
		~Http( void ) { return ; };
		Http( void ) { return ; };
		Http( Http const &obj ) { return ; };
};

#endif
