/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  HttpResponse.cpp                                     :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/05/07 17:02:47 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/05/15 17:18:17 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <fstream>
#include <utility>
#include <cstdlib>
#include <sys/stat.h>
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
			postResource(request);
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
	/*	*
	 *	Stringing resource
	 */
	std::string		file(_searchEndpoint(request.getPath()));
	std::string		line;
	std::ifstream	fileStream(file.c_str());

	if (!fileStream)
		throw (HttpException("File not found", 404));
	/*	*
	 *	Tha Body
	 */
	_body.clear();
	while (std::getline(fileStream, line))
		_body.append(line).append(1, '\n');
	fileStream.close();
	/*	*
	 *	Setting headers
	 */
	_header.insert(PairStr("Content-length",
				Libft::itos(static_cast<int>(_body.length()))));
	_header.insert(PairStr("Content-type", _fileType(file)));
	/*	*
	 *	Setting status line
	 */
	_code = 200;
}

void	HttpResponse::postResource(HttpRequest const &request)
{
	if (request.getPath().length() >= 255)
		throw (HttpException("Too large el mensaje ese", 414));
	/*	*
	 *	Creating resource
	 */
	std::string			file(_searchEndpoint(request.getPath()));
	std::string const	&body = request.getBody();
	std::ofstream		fileStream(file.c_str());

	if (fileStream.fail())
		throw (HttpException("Failed to create the file", 500));
	fileStream.write(body.c_str(), body.length());
	fileStream.close();
	/*	*
	 *	Tha Body
	 */
	_body.clear();
	/*	*
	 *	Setting headers
	 */
	_header.insert(PairStr("Content-length", "0"));
	/*	*
	 *	Setting status line
	 */
	_code = 201;
	//throw (HttpException("Not done yet", 400)); IS DONE NOW
}

void	HttpResponse::deleteResource(HttpRequest const &request)
{
	/*	*
	 *	Removing resource
	 */
	std::string	file(_searchEndpoint(request.getPath()));
	
	if (!_validFile(file))
		throw (HttpException("Real reasone inside validFile", 400));
	if (std::remove(file.c_str())== -1)
		throw (HttpException("Failed to remove the file", 500));
	/*	*
	 *	Tha Body
	 */
	_body.clear();
	/*	*
	 *	Setting headers
	 */
	_header.insert(PairStr("Content-length", "0"));
	/*	*
	 *	Setting status line
	 */
	_code = 200;
	//throw (HttpException("Yes done nos", 400));
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

std::string	HttpResponse::_fileType(std::string const &file)
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

std::string	HttpResponse::_searchEndpoint(std::string const &path)
{
	std::string	file;
	std::string::const_iterator	it(path.begin());

	//if (path.begin() != path.end() && path.front() == '/')
	if (it != path.end() && *it == '/')
		file.assign(path.substr(1));
	return (file);
}

bool	HttpResponse::_validFile(std::string const &file)
{
	if (file == "/" || file.find("html/") != 0)
		return (false);
	struct stat	sb;

	if (stat(file.c_str(), &sb) == -1)
	{
		if (errno == ENOENT)
			throw (HttpException("File not found", 404));
		if (errno == EACCES)
			throw (HttpException("Permission denied: Forbidden", 403));
		throw (HttpException("Failed stat function", 500));
	}
	if (!S_ISREG(sb.st_mode) && !S_ISDIR(sb.st_mode))
		throw (HttpException("Is nor a file nor a directory", 403));
	if (S_ISREG(sb.st_mode))
		return (true);
	return (false);
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
#include <cstdlib>
		std::cout << "Error occured: " << ex.what() << std::endl;
	}
}*/
