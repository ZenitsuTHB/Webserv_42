/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:11:48 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/03/18 14:52:39 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERDATA_HPP
#define SERVERDATA_HPP


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

  
- Lista de nombres de variables del archivo de configuracion

Servidor

	listen
	server_name
	default_server
	host
	root
	index
	error_page
	client_max_body_size

Rutas

	location
	root
	index
	allowed_methods
	return
	autoindex
	upload_enable
	upload_store

CGI

	cgi_extension
	cgi_pass
	fatcgi_pass
	fatcgi_param

Global

	worker_processes
	log_format
	access_log
	error_log
	error_log_level
	timeout
 

struct RouteConfig
{
	bool								directoryListening = autoindex
	bool								allowFileUploads = allow_file_uploads
	size_t								maxBodySize = client_max_body_size
	std::string							redirection = return, redirect
	std::string							root = root
	std::string							defaultFile = index
	std::string							uploadDirectory = upload_directory
	std::vector<std::string>			allowedMethods = allowed_methods, limit_except
	std::map<std::string, std::string>	cgiHandlers = fastcgi_pass, cgi_handlers
};

struct	ServerConfig
{
	bool								isDefault = default_server
	size_t								clientBodySizeLimit = client_max_body_size
	std::string							host = host
	std::vector<int>					ports = listen
	std::vector<std::string>			serverNames = server_name
	std::map<std::string, RouteConfig>	routes = location
	std::map<int, std::string>			errorPages = error_page
};

struct	LogConfig
{
	int									timeout = timeout
	std::string							logFormat = log_format
	std::string							accessLog = access_log
	std::string							errorLog = error_log
	std::string							errorLogLevel = error_log_level
};


*/

#include <iostream>
#include <vector>
#include <map>
#include <set>

struct RouteConfig
{
	bool								directoryListening;
	bool								allowFileUploads;
	size_t								maxBodySize;
	std::string							redirection;
	std::string							root;
	std::string							defaultFile;
	std::string							uploadDirectory;
	std::vector<std::string>			allowedMethods;
	std::map<std::string, std::string>	cgiHandlers;
};

struct	ServerConfig
{
	bool								isDefault;
	size_t								clientBodySizeLimit;
	std::string							host;
	std::set<int>						ports;
	std::vector<std::string>			serverNames;
	std::map<int, std::string>			errorPages;
	std::map<std::string, RouteConfig>	locations;
};

struct	LogConfig
{
	int									timeout;
	std::string							logFormat;
	std::string							accessLog;
	std::string							errorLog;
	std::string							errorLogLevel;
};

struct	ConfigFile
{
	std::vector<ServerConfig>	servers;
	LogConfig					logging;
};

enum	TypeData
{
	ROUTE = 0,
	VAR,
	TOKEN,
	ERROR
};

class ServerData
{
	public:

		ServerData(char const *path);
		~ServerData();

		ConfigFile	getConfi() const;

	private:

		ConfigFile	_confi;
		bool		_end;

		void		parserConfig(std::ifstream &file);
		void		parserServer(std::ifstream &file, bool token);
		//void		parserRoute(std::ifstream &file, std::string data);
		void		parserVar(std::string const &var, std::string value, ServerConfig &server);
		
		bool		isToken(std::ifstream &file);
		TypeData	whatIsThis(std::string var, std::string &data, bool &exitBlock);
};

#endif
