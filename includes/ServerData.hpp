/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:11:48 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/03/20 14:42:03 by adrmarqu         ###   ########.fr       */
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

*/

/*
 
struct	RouteConfig
{
	std::string					path;				location path {
	std::bitset<3> 				allowedMethods;			allowed_methods		GET POST DELETE
	std::string					rootDirectory;			root 				/var/www
	bool						directoryListening;		directory_listing	off/on
	std::string					defaultFile;			default_file		index.html
	std::string					redirect;				redirect			/new-path
	bool						cgiEnabled;				cgi_enabled			on/off
	std::string					cgiPath;				cgi_path			/usr/bin/php-cgi
	std::string					cgiExtension;			cgi_extension		.php
	bool						uploadEnabled;			upload_enable		on/off
	std::string					uploadStore;			upload_store		/var/www/uploads
};													}

struct	ServerConfig
{													server {
	std::set<int>				ports;					listen				8080
	std::string					host;					host				127.0.0.1
	std::string					serverName;				server_name			example.com
	std::string					root;					root				/var/www
	std::string					index;					index				index.html
	size_t						clientMaxBodySize;		client_max_body_size 100M
	std::map<int, std::string>	errorPages;				error_page			404 /404.html
	std::vector<RouteConfig>	routes;					location / {
};													}
 
 
 */

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <bitset>

enum	Methods
{
	GET = 0,
	POST,
	DELETE
};

struct	RouteConfig
{
	std::string					path;
	std::bitset<3> 				allowedMethods;
	std::string					rootDirectory;
	bool						autoindex;
	bool						directoryListening;
	std::string					defaultFile;
	std::string					redirect;
	bool						cgiEnabled;
	std::string					cgiPath;
	std::string					cgiExtension;
	bool						uploadEnabled;
	std::string					uploadStore;
};

struct	ServerConfig
{
	std::set<int>				ports;
	std::string					host;
	std::string					serverName;
	std::string					root;
	std::string					index;
	size_t						clientMaxBodySize;
	std::map<int, std::string>	errorPages;
	std::vector<RouteConfig>	routes;
};

class ServerData
{
	public:

		ServerData(char const *path);
		~ServerData();

	private:

		std::vector<std::string>	_serverData;
		std::vector<ServerConfig>	_servers;
		bool						_error;

		void	setDataFile(std::ifstream &file);

		void	parserData();
		void	parserServer(int &idx);
		void	parserRoute(int &idx, ServerConfig &server);
		bool	parserRouteVar(int &idx, RouteConfig &route);
		bool	parserVar(int &idx, ServerConfig &server);

		bool	methods(int &idx, RouteConfig &route);

};

#endif
