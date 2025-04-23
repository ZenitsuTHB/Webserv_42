/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  HttpRequest.cpp                                      :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/04/23 22:39:03 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/04/23 22:39:19 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpRequest.hpp"

HttpRequest::~HttpRequest(void)
{
	return ;
}

HttpRequest::HttpRequest(void)
{
	return ;
}

HttpRequest::HttpRequest(HttpRequest const &obj)
{
	*this = obj;
}

HttpRequest	&HttpRequest:: operator = (HttpRequest const &obj)
{
	if (this != &obj)
	{
	}
	return (*this);
}
