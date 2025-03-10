/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfiData.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:11:48 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/03/10 19:55:20 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIDATA_HPP
#define CONFIDATA_HPP

#include <iostream>

/*

error_page

	error_page <codigo_error> <página_de_error>; --> error_page 404 /404.html
	error_page <codigo_error> <codigo_error> <página_de_error>; --> error_page 500 502 503 /50x.html


listen

	listen <dirección_ip>:<puerto>; --> listen 8080;
	listen <puerto> default_server; -->	listen 127.0.0.1:8080 ssl;
	listen <dirección_ip>:<puerto> ssl; --> listen 443 default_server;


allow_methods

	allow_methods <metodo1> <metodo2> <metodo3> --> allow_methods GET POST DELETE


location

	location <path> { <configuraciones> } --> 
		location /
		{
			root /var/www/html;
			index	index.html;
		}

	location <path> <otros parametros> -->

	location /upload {
		upload_directory /var/uploads;
		allow_methods POST;
	}


fastcgi_param

	fastcgi_param <parametro1> <valor1>; --> fastcgi_param SCRIPT_FILENAME /usr/local/bin/php;
	fastcgi_param <parametro2> <valor2>; --> fastcgi_param QUERY_STRING $query_string;


server_name

	server_name <nombre1> <nombre2> <nombre3> --> server_name example.com www.example.com


access_log

	access_log <archivo_log> <formato>; --> access_log /var/log/nginx/access.log main;
	access_log <archivo_log>; --> access_log /dev/null;


error_log

	error_log <archivo_log> <nivel>; --> error_log /var/log/nginx/error.log warn;
	error_log <archivo_log>; --> error_log /dev/null;

*/

struct	Server
{
	int					*listen;
	std::string			*serverName;
	std::vector<int>	errorCode;
	std::string			*errorPage;
	std::string			root;
	std::string			index;
};

struct	Location
{
	std::string			path;
	std::string			root;
	std::string			index;
	std::string			fastcgiPass;
	std::string			fastcgiParam;
	std::string			fastcgiParamValue;
	std::string			uploadDirectory;
	// allow methods
	bool				_get;
	bool				_post;
	bool				_delete;
	std::string			redirect;
	bool				directoryListing;
	std::string			cgiExtension;
	size_t				maxBodySize;
	std::string			defaultFile
};

struct	Global
{
	int					port;
	size_t				serverNamesHashMaxSize;
	size_t				serverNamesHashBucketSize;
	size_t				clientMaxBodySize;
};

struct	Other
{
	std::string			logFormat;
	std::string			accessLog;
	std::string			accessLogFormat;
	std::string			errorLog;
	std::string			errorLogLevel;
	std::string			workerProcesses;
	int					timeout;
};

struct	Config
{
	Server			server;
	Location		location;
	Global			global;
	Other			others;
};

template <typename T>
class	ConfiData
{
	public:

		ConfiData(char **conf);
		~ConfiData();

		

	private:

		Config		_config;
		std::map<T>	_data;
};

#endif
