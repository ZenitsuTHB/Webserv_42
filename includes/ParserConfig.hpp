/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:57:58 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/05/08 13:12:01 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSERCONFIG_HPP
#define PARSERCONFIG_HPP

/*

server

listen: puerto y host
server_name: Nombre del servidor
error_page: Paginas personalizadas para codigos de error HTTP
client_max_body_size: Size maximo del cuerpo de request (10KB - 100MB)
root: Ruta raiz del sistema
index: Archivo a servir si se accede a un directorio (como index.html)
return: Redirecciones HTTP

route
	
root: Raiz especifica de esta ruta
index: archivo por defecto al acceder a esta ruta
autoindex: Activa o desactiva el listado automatico del directorio
limit_except: Restringe los metodos HTTP permitidos (GET, POST, DELETE)
error_page: Puedes redifinir errores especificos en rutas concertas
client_max_body_size: Size especifico de una ruta
return: Redirecciones HTTP
cgi_pass: Ejecutar cgi
cgi_extension: extension

*/

#include <iostream>
#include <vector>
#include <fstream>
#include "ServerConfig.hpp"

class ParserConfig
{
	public:
	
		ParserConfig(std::string input);
		~ParserConfig();

		std::vector<ServerConfig> const		&getServers() const;
		unsigned int						size() const;

	private:

		std::vector<VectorStr>		_configFile;
		std::vector<ServerConfig>	_servers;

		void	sentError(std::string msg);
		
		bool	isEmpty(std::string const &line) const;
		void	deleteComment(std::string &line);
		void	getDataLine(std::string line, std::string &var, VectorStr &values);

		void	addServer(std::ifstream &file);
		void	addRoute(std::ifstream &file, VectorStr const &path, ServerConfig &server);
		void	addServerVar(std::string const &var, VectorStr values, ServerConfig &server);
		void	addRouteVar(std::string const &var, VectorStr values, RouteConfig &route);

};

#endif
