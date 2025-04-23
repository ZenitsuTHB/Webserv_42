/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  HttpRequest.hpp                                      :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/04/23 22:39:01 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/04/23 22:39:17 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

class	HttpRequest
{
	public:
		~HttpRequest( void );
		HttpRequest( void );
		HttpRequest( HttpRequest const &obj );

		HttpRequest	& operator = ( HttpRequest const &obj );

	private:
};

#endif
