/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  HttpRequest.cpp                                      :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/04/23 22:39:03 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/05/08 18:48:20 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/HttpRequest.hpp"
#include "../../includes/Libft.hpp"
#include "../../includes/exceptions.hpp"
#include <iostream>

HttpRequest::~HttpRequest(void)
{
	return ;
}

HttpRequest::HttpRequest(std::string const &req)
{
	PairStr	headBody = Libft::splitInTwo(req, CRLF CRLF);
	if (headBody.first.empty())
		throw (HttpException("Empty request", 400));
	/*	*
	 *	Body part
	 */
	if (!headBody.second.empty())
		_body = headBody.second;
	VecStr	reqSplit = Libft::split(headBody.first, "\n");
	//VecStr	reqSplit = Libft::split(headBody.first, CRLF);
	if (reqSplit.empty())
		throw (HttpException("Empty request line", 400));
	/*	*
	 *	Request Line
	 */
	VecStr	reqLine = Libft::split(reqSplit.front(), SPACE);
	if (reqLine.size() != 3)
		throw (HttpException("Bad request line", 400));
	_method = Http::toMethod(reqLine.front());
	if (!_method)
		throw (HttpException("Unkwon method", 400));
	_path = reqLine[1];
	_version = reqLine[2];
	reqSplit.erase(reqSplit.begin());
	/*	*
	 *	Headers part
	 */
	for (VecStr::iterator it = reqSplit.begin(); it != reqSplit.end(); ++it)
	{
		PairStr	header(Libft::splitInTwo(*it, ":"));
		Libft::trim(header.first, SPACE);
		Libft::trim(header.second, SPACE);
		if (header.first.empty())
			throw (HttpException("Wrong header format", 400));
		_header.insert(header);
	}
}

void	HttpRequest::print(void) const
{
	std::cout << "REQUEST IS" << std::endl;
	std::cout << "Method: " << Http::toString(_method) << std::endl;
	std::cout << "Resource path: " << _path << std::endl;
	std::cout << "HTTP Version: " << _version << std::endl;
	std::cout << "Headers:" << std::endl;
	for (Headers::const_iterator it = _header.begin();
			it != _header.end();
			++it)
	{
		std::cout << " Key: \"" << it->first << "\"" << std::endl;
		std::cout << "  Value: \"" << it->second << "\"" << std::endl;
	}
	std::cout << "Body(" << _body.length() << "): " << _body << std::endl;
}

HttpRequest::HttpRequest(HttpRequest const &obj)
{
	*this = obj;
}

HttpRequest	&HttpRequest:: operator = (HttpRequest const &obj)
{
	if (this != &obj)
	{
		_method = obj._method;
		_path = obj._path;
		_version = obj._version;
		_header = obj._header;
		_body = obj._body;
	}
	return (*this);
}
/*
int	main(void)
{
	std::string	str =
		"GET /500 HTTP/3\r\n"
		"Host: http.cat\r\n"
		"User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:135.0)     \r\n"
		"Accept: text/html,application/xhtml+xml,application/xml;\r\n"
		"Accept-Language: en-US,en;q=0.5\r\n"
		"Accept-Encoding: gzip, deflate, br, zstd\r\n"
		"Connection: keep-alive\r\n"
		"Cookie: _ga=GA1.2.549077782.1743611458; \r\n"
		"Upgrade-Insecure-Requests: 1\r\n"
		"Sec-Fetch-Dest: document\r\n"
		"Sec-Fetch-Mode: navigate\r\n"
		"Sec-Fetch-Site: none\r\n"
		"Sec-Fetch-User: ?1\r\n"
		"\t\v\r\f Priority\t\v\r\f :\t\v\r\f u=0, i\t\v\r\f \r\n\r\n"
		"12345678901234567890123456789012345678901234567890";
	try
	{
		HttpRequest	request(str);

		request.print();
	}
	catch (std::exception const &ex)
	{
		std::cout << "Error occured: " << ex.what() << std::endl;
	}
}*/
