/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  Http.cpp                                             :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/04/23 21:18:52 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/05/06 21:48:40 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Http.hpp"

Method	Http::toMethod(std::string const &method)
{
	Method	ret = NO_METHOD;

	if (method == "GET")
		ret = GET;
	else if (method == "POST")
		ret = POST;
	else if (method == "DELETE")
		ret = DELETE;
	return (ret);
}

std::string	Http::toString(Method method)
{
	std::string	ret;

	switch (method)
	{
		case GET:
			ret = "GET";
			break ;
		case POST:
			ret = "POST";
			break ;
		case DELETE:
			ret = "DELETE";
			break ;
		default:
			ret = "NO_TYPE";
	}
	return (ret);
}
