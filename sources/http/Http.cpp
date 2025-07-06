/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  Http.cpp                                             :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/04/23 21:18:52 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/07/05 18:11:27 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Http.hpp"

std::map< int, std::string >	createErrorCodes(void)
{
	std::map< int, std::string >	map;
    map[100] = "Continue";
    map[101] = "Switching Protocols";
    map[102] = "Processing";
    map[103] = "Early Hints";
    map[200] = "SuckSexs";
    map[201] = "Created";
    map[202] = "Accepted";
    map[203] = "Non-Authoritative Information";
    map[204] = "No Content";
    map[205] = "Reset Content";
    map[206] = "Partial Content";
    map[207] = "Multi-Status";
    map[208] = "Already Reported";
    map[214] = "Transformation Applied";
    map[226] = "IM Used";
    map[300] = "Multiple Choices";
    map[301] = "Moved Permanently";
    map[302] = "Found";
    map[303] = "See Other";
    map[304] = "Not Modified";
    map[305] = "Use Proxy";
    map[307] = "Temporary Redirect";
    map[308] = "Permanent Redirect";
    map[400] = "Bad Request";
    map[401] = "Unauthorized";
    map[402] = "Payment Required";
    map[403] = "Forbidden";
    map[404] = "Not Found";
    map[405] = "Method Not Allowed";
    map[406] = "Not Acceptable";
    map[407] = "Proxy Authentication Required";
    map[408] = "Request Timeout";
    map[409] = "Conflict";
    map[410] = "Gone";
    map[411] = "Length Required";
    map[412] = "Precondition Failed";
    map[413] = "Payload Too Large";
    map[414] = "Request-URI Too Long";
    map[415] = "Unsupported Media Type";
    map[416] = "Request Range Not Satisfiable";
    map[417] = "Expectation Failed";
    map[418] = "I’m a teapot";
    map[419] = "Page Expired";
    map[420] = "Enhance Your Calm";
    map[421] = "Misdirected Request";
    map[422] = "Unprocessable Entity";
    map[423] = "Locked";
    map[424] = "Failed Dependency";
    map[425] = "Too Early";
    map[426] = "Upgrade Required";
    map[428] = "Precondition Required";
    map[429] = "Too Many Requests";
    map[431] = "Request Header Fields Too Large";
    map[444] = "No Response";
    map[450] = "Blocked by Windows Parental Controls";
    map[451] = "Unavailable For Legal Reasons";
    map[495] = "SSL Certificate Error";
    map[496] = "SSL Certificate Required";
    map[497] = "HTTP Request Sent to HTTPS Port";
    map[498] = "Token expired/invalid";
    map[499] = "Client Closed Request";
    map[500] = "Internal Server Error";
    map[501] = "Not Implemented";
    map[502] = "Bad Gateway";
    map[503] = "Service Unavailable";
    map[504] = "Gateway Timeout";
    map[506] = "Variant Also Negotiates";
    map[507] = "Insufficient Storage";
    map[508] = "Loop Detected";
    map[509] = "Bandwidth Limit Exceeded";
    map[510] = "Not Extended";
    map[511] = "Network Authentication Required";
    map[521] = "Web Server Is Down";
    map[522] = "Connection Timed Out";
    map[523] = "Origin Is Unreachable";
    map[525] = "SSL Handshake Failed";
    map[530] = "Site Frozen";
    map[599] = "Network Connect Timeout Error";
	return (map);
}

std::map< std::string, std::string >	createFileTypes(void)
{
	std::map< std::string, std::string >	map;

	map["html"] =	"text/html";
	map["css"] =	"text/css";
	map["js"] =		"application/javascript";
	map["json"] =	"application/json";
	map["xml"] =	"application/xml";
	map["pdf"] =	"application/pdf";
	map["zip"] =	"application/zip";
	map["png"] =	"image/png";
	map["jpeg"] =	"image/jpeg";
	map["jpg"] =	"image/jpg";
	map["gif"] =	"image/gif";
	map["webp"] =	"image/webp";
	map["mpeg"] =	"audio/mpeg";
	map["ogg"] =	"audio/ogg";
	map["mp4"] =	"video/mp4";
	map["webm"] =	"video/webm";
	map["ogg"] =	"video/ogg";
	return (map);
}

std::map< int, std::string > const	Http::_errorCode = createErrorCodes();
std::map< std::string, std::string > const Http::_fileType = createFileTypes();

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

std::string	Http::getError(int idx)
{
	std::map< int, std::string >::const_iterator	it = _errorCode.find(idx);
	std::string	ret;

	if (it != _errorCode.end())
		ret.assign(it->second);
	else
		ret.assign("Unknown error code");
	return (ret);
}

std::string	Http::getFileType(std::string const key)
{
	std::map< std::string, std::string >::const_iterator	it
		= _fileType.find(key);
	std::string	ret;

	if (it != _fileType.end())
		ret.assign(it->second);
	else
		ret.assign("text/plain");
	return (ret);
}
