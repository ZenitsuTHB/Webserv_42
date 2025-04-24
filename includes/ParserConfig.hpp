/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:57:58 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/04/24 14:11:23 by adrmarqu         ###   ########.fr       */
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

*/

#include <iostream>
#include <vector>

class ParserConfig
{
	public:
	
		ParserConfig(std::string input);
		~ParserConfig();

	private:

		std::vector<std::string>	_configFile;
};

#endif
