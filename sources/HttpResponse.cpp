/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  HttpResponse.cpp                                     :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/05/07 17:02:47 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/05/09 17:46:09 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <utility>
#include "../includes/HttpResponse.hpp"
#include "../includes/exceptions.hpp"
#include "../includes/Libft.hpp"

HttpResponse::~HttpResponse(void)
{
	return ;
}

HttpResponse::HttpResponse(HttpRequest const &request): _version("HTTP/1.1")
{
	switch (request.getMethod())
	{
		case GET:
			getResource(request);
			break ;
		case POST:
			postResorce(request);
			break ;
		case DELETE:
			deleteResource(request);
			break ;
		default:
			throw (HttpException("Unkwon method", 400));
	}
}

HttpResponse::HttpResponse(HttpResponse const &obj)
{
	*this = obj;
}

HttpResponse	&HttpResponse:: operator = (HttpResponse const &obj)
{
	if (this != &obj)
	{
	}
	return (*this);
}

void	HttpResponse::getResource(HttpRequest const &request)
{
	std::string	file(HttpResponse::parsePath(request.getPath()));
	std::string			line;
	std::ifstream		fileStream(file.c_str());

	if (!fileStream)
		throw (HttpException("File not found", 404));
	/*	*
	 *	Tha Body
	 */
	_body.clear();
	while (std::getline(fileStream, line))
		_body.append(line).append(1, '\n');
	/*	*
	 *	Setting headers
	 */
	_header.insert(std::pair< std::string, std::string >("Content-length",
				Libft::itos(static_cast<int>(_body.length()))));
	_header.insert(std::pair< std::string, std::string >("Content-type",
				HttpResponse::fileType(file)));
	/*	*
	 *	Setting status line
	 */
	_code = 200;
}

void	HttpResponse::postResorce(HttpRequest const &request)
{
	(void)request;
	throw (HttpException("Not done yet", 400));
}

void	HttpResponse::deleteResource(HttpRequest const &request)
{
	(void)request;
	throw (HttpException("Not done yet", 400));
}

std::string	HttpResponse::generate(void) const
{
	std::string response;

	/*	*
	 *	Adding status line
	 */
	response.append(_version).append(1, ' ').append(Libft::itos(_code));
	response.append(1, ' ').append(Http::getError(_code));
	response.append(CRLF);
	/*	*
	 *	Adding headers
	 */
	for (Headers::const_iterator it = _header.begin();
			it != _header.end(); ++it)
	{
		response.append(it->first).append(" : ").append(it->second);
		response.append(CRLF);
	}
	/*	*
	 *	Adding body
	 */
	response.append(CRLF);
	response.append(_body);
	return (response);
}

std::string	HttpResponse::fileType(std::string const &file)
{
	std::string	type;
	if (file.find(".html") != std::string::npos)
		type = Http::getFileType("html");
	else if (file.find(".css") != std::string::npos)
		type = Http::getFileType("css");
	else
		type.assign("text/plain");
	return (type);
}

std::string	HttpResponse::parsePath(std::string const &path)
{
	std::string	file;
	std::string::const_iterator	it(path.begin());

	//if (path.begin() != path.end() && path.front() == '/')
	if (it != path.end() && *it == '/')
		file.assign(path.substr(1));
	return (file);
}
/*
#include <iostream>

int	main(void)
{
	std::string	str =
		"GET /html/index.html HTTP/3\r\n"
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
		HttpRequest		request(str);

		request.print();
		HttpResponse	response(request);

		std::cout << "Response:" << std::endl << response.generate();
	}
	catch (HttpException const &ex)
	{
		std::cout << "Http error uccored: " << ex.what() << " with code: "
			<< ex.whatCode() << std::endl;
	}
	catch (std::exception const &ex)
	{
		std::cout << "Error occured: " << ex.what() << std::endl;
	}
}*/
