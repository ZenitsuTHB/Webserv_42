/* ************************************************************************** */
/*                                                         :::      ::::::::  */
/*  HttpResponse.cpp                                     :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/05/07 17:02:47 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/06/11 15:55:52 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <fstream>
#include <utility>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "../../includes/HttpResponse.hpp"
#include "../../includes/exceptions.hpp"
#include "../../includes/Libft.hpp"

/*
=======================
Auxiliar Functions
=======================
*/
static BaseConfig const	*assignRoute(ServerConfig const &conf,
		std::string const &path)
{
	try 
	{
		return (&conf.getRoute(path));
	}
	catch (...)
	{
		std::cerr << "This is Adri's fault" << std::endl;
	}
	return (&conf);
}

//QUITAR SI NO HACE FALTA
std::string	getPath(BaseConfig const *conf)
{
	std::string	path;

	path.assign(conf->getRoot());
	if (RouteConfig const *ptr = dynamic_cast<RouteConfig const *>(conf))
		path.append(ptr->getPath());
	else
		path.append(1, '/');
	return (path);
}

//BORRAR?
bool	isAutoindex(BaseConfig const *conf)
{
	if (RouteConfig const *ptr = dynamic_cast<RouteConfig const *>(conf))
		return (ptr->isAutoindex());
	return (true);
}

/*
=======================
-tructors
=======================
*/
HttpResponse::~HttpResponse(void)
{
	return ;
}

HttpResponse::HttpResponse(HttpRequest const &req, ServerConfig const &conf):
	_version("HTTP/1.1"), _index(false), _serverConf(conf);
{
	_route = assignRoute(conf, req.getPath());
	switch (req.getMethod())
	{
		case GET:
			getResource(req);
			break ;
		case POST:
			postResource(req);
			break ;
		case DELETE:
			deleteResource(req);
			break ;
		default:
			throw (HttpException("<Response> Unkwon method", 400));
	}
}

/*
HttpResponse::HttpResponse(HttpResponse const &obj)
{
	*this = obj;
}*/

/*
=======================
Member methods
=======================
*/
void	HttpResponse::searchGETendPoint(std::string &file)
{
	hola
	struct stat	sb;

	/*	*
	 *	Add root to the file path
	 */
	file.insert(0, _route->getRoot());
	/*	*
	 *	Check file permissions
	 */
	std::cout << "stat function argument : " << file << std::endl;
	if (stat(file.c_str(), &sb) == -1)
	{
		/*	*
		 *	Errors while looking for file
		 */
		switch (errno)
		{
			case ENOTDIR:// PODRIA QUITAR LOS BARRA DE DESPUES
				//fallthrow
			case ENOENT:
				throw (HttpException("GET st NOTDIRENT File not found", 404));
				break ;
			case EACCES:
				throw (HttpException("GET st ACCES Permission denied", 403));
				break ;
			case ENAMETOOLONG:
				//fallthrow
			case ELOOP:
				throw (HttpException("GET st TOOLONGLOOP Name too long", 414));
				break ;
			default:
				throw (HttpException("GET st dflt Server internal error", 500));
		}
	}
	/*	*
	 *	If it is a regular file and has read permissions
	 */
	if (S_ISREG(sb.st_mode))
	{
		if (!(sb.st_mode & S_IRUSR))
			throw (HttpException("GET reg read Permission denaid", 403));
		std::cout << "File: " << file << std::endl;
		return ;
	}
	/*	*
	 *	If it is a folder
	 */
	else if (S_ISDIR(sb.st_mode))
	{
		/*	*
		 *	Search for index file
		 */
		std::string indexFile;

		for (VecStr::const_iterator it = _route->getIndexFiles().begin();
				it != _route->getIndexFiles().end(); ++it)
		{
			indexFile.assign(file + *it);
			if (access(indexFile.c_str(), R_OK) == -1)
				continue ;
			file.assign(indexFile);
			std::cout << "File: " << file << std::endl;
			return ;
		}
		/*	*
		 *	or create index
		 */
		if (_route->isAutoindex())
		//if (isAutoindex(_route))
		{
			std::cout << "File: " << file << std::endl;
			_index = true;
			return ;
		}
	}
	/*	*
	 *	If it didn't return, error
	 */
	std::cout << "File: " << file << std::endl;
	throw (HttpException("GET nothing File not faun", 404));
}

int	HttpResponse::checkFile(std::string const &file)
{
	hola
	struct stat	sb;
	int			ret;

	stat.st_mode = 0;
	if (stat(file.c_str(), &sb) == -1)
	{
		switch (errno)
		{
			case ENOTDIR:
				return (ENOTDIR);
			case ENOENT:
				return (ENOENT):
			case EACCES:
				return (EACCES):
			case ENAMETOOLONG:
				return (ENAMETOOLONG):
			case ELOOP:
				return (ELOOP):
			default:
				return (0);
		}
	}
	ret = 0;
	if (S_ISREG(sb.st_mode))
		return (sb.st_mode);
	else if (S_ISDIR(sb.st_mode))
		return (sb.st_mode);
	return (0);
}

void	HttpResponse::getCgi(HttpResponse const &request)
{
	int	fileStat;

	if (!isCgiAllowed(request.getPath(), _serverConf))
		throw (HttpException("getCGI not allowed here", 403));
	fileStat = checkFile(
}

void	HttpResponse::getResource(HttpRequest const &request)
{
	std::string		fileName = request.getPath();

	if (isCgi(fileName))
		return (getCgi(request));
	/*	*
	 *	Search the Endpoint
	 */
	searchGETendPoint(fileName);
	/*	*
	 *	Tha Body
	 */
	if (_index)
		_body = _indexFolder(fileName);
	else
		_body = Libft::readFile(fileName);
	/*	*
	 *	Setting headers
	 */
	_header.insert(PairStr("Content-length",
				Libft::itos(static_cast<int>(_body.length()))));
//	if (_index)
//		_header.insert(PairStr("Content-type", "text/html"));
//	else
	_header.insert(PairStr("Content-type", _fileType(fileName)));
	/*	*
	 *	Setting status line
	 */
	_code = 200;
}

void	HttpResponse::searchPOSTendPoint(std::string &file)
{
	struct stat	sb;

	file.insert(0, _route->getRoot());
	if (stat(file.c_str(), &sb) == -1)
	{
		/*	*
		 *	Errors while looking for file
		 */
		switch (errno)
		{
			case ENOTDIR:// PODRIA QUITAR LOS BARRA DE DESPUES
				throw (HttpException("POST st NOTDIR File not found", 404));
				break ;
			case ENOENT:
				return ;
			case EACCES:
				throw (HttpException("POST st ACCES Permission denied", 403));
				break ;
			case ENAMETOOLONG:
				//fallthrow
			case ELOOP:
				throw (HttpException("POST st TOOLONGLOOP Name too long", 414));
				break ;
			default:
				throw (HttpException("POST st dft Server internal error", 500));
		}
	}
	if (S_ISDIR(sb.st_mode))
		throw (HttpException("POST dir File not found", 404));
	if (S_ISREG(sb.st_mode) && !(sb.st_mode & S_IWUSR))
		throw (HttpException("POST reg write Permission denaid", 403));
	std::cout << "File: " << file << std::endl;
}

void	HttpResponse::postResource(HttpRequest const &request)
{
	/*	*
	 *	Creating resource
	 */
	std::string			fileName(request.getPath());
	searchPOSTendPoint(fileName);

	std::string const	&body = request.getBody();
	std::ofstream		file(fileName.c_str(), std::ios::binary);

	// Plantear si hay que hacer lo del parámetro Burundy en el Content-type
	if (file.fail())
		throw (HttpException("POST stream Failed to create the file", 500));
	file.write(body.c_str(), body.length());
	file.close();
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

void	HttpResponse::searchDELETEendPoint(std::string &file)
{
	struct stat	sb;

	file.insert(0, _route->getRoot());
	if (stat(file.c_str(), &sb) == -1)
	{
		/*	*
		 *	Errors while looking for file
		 */
		switch (errno)
		{
			case ENOTDIR:// PODRIA QUITAR LOS BARRA DE DESPUES
				//fallthrow
			case ENOENT:
				std::cout << "File: " << file << std::endl;
				throw (HttpException("DEL st NODIRENT File not found", 404));
			case EACCES:
				throw (HttpException("DEL st ACCES Permission denied", 403));
				break ;
			case ENAMETOOLONG:
				//fallthrow
			case ELOOP:
				throw (HttpException("DEL st TOOLONGLOOP Name too long", 414));
				break ;
			default:
				throw (HttpException("DEL st dflt Server internal error", 500));
		}
	}
	// If it is a directory should the directory files be removed?
	if (S_ISDIR(sb.st_mode))
		throw (HttpException("DEL dir File not found", 404));
	if (!S_ISREG(sb.st_mode))
		throw (HttpException("DEL not reg File not faun", 404));
	std::cout << "File: " << file << std::endl;
}

void	HttpResponse::deleteResource(HttpRequest const &request)
{
	/*	*
	 *	Removing resource
	 */
	std::string	file(request.getPath());
	
	searchDELETEendPoint(file);
	if (std::remove(file.c_str())== -1)
		throw (HttpException("DEL remove Failed to remove the file", 500));
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
std::string	HttpResponse::_indexFolder(std::string const &folder)
{
	(void)folder;
	return ("Folder Indexado\r\n");
}

HttpResponse	&HttpResponse:: operator = (HttpResponse const &obj)
{
	if (this != &obj)
	{
	}
	return (*this);
}

#include <stdexcept>
void	HttpResponse::getHeaderBody(
		int code,
		std::string &body,
		Headers &header,
		ServerConfig const &config
		)
{
	std::string	page;
	page = const_cast<ServerConfig *>(&config)->getErrorPage(code);
	std::cerr << "Im reading this: " << page << std::endl;
	try
	{
		struct stat	sb;
		if (stat(page.c_str(), &sb) == -1 || !S_ISREG(sb.st_mode))
			throw (std::invalid_argument("Page not found"));
		body = Libft::readFile(page);
	}
	catch (std::exception const &ex)
	{
		body.clear();
		std::cerr << "Failed to get error Page" << ex.what() << std::endl;
	}
	header.insert(PairStr("Content-type", HttpResponse::_fileType(page)));
	header.insert(PairStr("Content-length",
				Libft::itos(static_cast<int>(body.length()))));
}

std::string	HttpResponse::createError(int code, ServerConfig const &conf)
{
	std::string	response;
	std::string	body;
	Headers		header;

	/*	*
	 *	Status Line
	 */
	response.append("HTTP/1.1 ").append(Libft::itos(code)).append(1, ' ');
	response.append(Http::getError(code)).append(CRLF);
	/*	*
	 *	Headers
	 */
	getHeaderBody(code, body, header, conf);
	for (Headers::iterator it = header.begin(); it != header.end(); ++it)
		response.append(it->first).append(": ").append(it->second).append(CRLF);
	response.append(Libft::itos(static_cast<int>(body.length()))).append(CRLF);
	response.append(CRLF);
	/*	*
	 *	Body
	 */
	response.append(body);
	return (response);
}

bool	HttpResponse::isCgiAllowed(std::string const &command,
		ServerConfig const &server)
{
	RouteConfig	*route;

	route = server.getLocation(command);

	if (!route->isCgiAllowed())
		return false;
	
	VectorStr const ext = route->getCgiExtensions();

	for (size_t i = 0; i < ext.size(); i++)
	{
		int	len = command.length() - ext[i].length();
		if (len >= 0 && ext[i] == command.substr(len))
			return (true);
	}
	return false;
}

std::string HttpResponse::executeCgi(std::string const &command)
{
	int	pipefd[2];
	std::string	cgiOut;

	if (pipe(pipefd) == -1)
	{
		std::cerr << "Pipe error: " << strerror(errno) << std::endl;
		throw (HttpException("CGI pipe error", 500));
	}

	pid_t	pid = fork();
	if (pid < 0)
	{
		std::cerr << "Fork failed: " << strerror(errno) << std::endl;
		close(pipefd[0]);
		close(pipefd[1]);
		throw (HttpException("CGI fork error", 500));
	}

	if (pid == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);

		char	*argv[] = { const_cast<char*>(command.c_str()), NULL };
		char	*envp[] = {
			const_cast<char*>("REQUEST_METHOD=GET"),
			const_cast<char*>("GATEWAY_INTERFACE=CGI/1.1"),
			const_cast<char*>("SCRIPT_FILENAME="),
			NULL
		};

		execve(command.c_str(), argv, envp);
		std::cerr << "Execve failed: " << strerror(errno) << std::endl;
		exit(127);
	}
	close(pipefd[1]);

	char		buffer[1024];
	ssize_t		count;

	while ((count = read(pipefd[0], buffer, sizeof(buffer))) > 0)
		cgiOut.append(buffer, count);

	close(pipefd[0]);
	int	ret = 0;
	waitpid(pid, &ret, 0);
	if (ret)
		throw (HttpException("CGI failed to execute", 500));
	return (cgiOut);
}

bool	HttpResponse::isCgi(std::string const &command)
{
	if (command.find(".cgi") != std::string::npos
		|| command.find(".php") != std::string::npos
		|| command.find(".py") != std::string::npos
		|| command.find(".pl") != std::string::npos
		|| command.find(".sh") != std::string::npos
		|| command.find(".exe") != std::string::npos
		|| command.find("/cgi-bin/") != std::string::npos)
		return (true);
	return (false);
}

#include <iostream>
#include <cstdlib>
#include "../../includes/ServerConfig.hpp"
#include "../../includes/ParserConfig.hpp"

/*int	main(int argc, char **av)
{
	if (argc != 2)
		return (1);
	std::string	str =
		"GET /TEST/this HTTP/3\r\n"
		"Host: http.cat\r\n"
		"Accept: text/html,application/xhtml+xml,application/xml;\r\n"
		"Accept-Language: en-US,en;q=0.5\r\n"
		"Accept-Encoding: gzip, deflate, br, zstd\r\n"
		"Connection: keep-alive\r\n"
		"\t\v\r\f Priority\t\v\r\f :\t\v\r\f u=0, i\t\v\r\f \r\n\r\n"
		"12345678901234567890123456789012345678901234567890";

		std::string	resString;
		ParserConfig					parser(av[1]);
	try
	{
		ServerConfig const	server = parser.getServers().front();
		//std::vector<ServerConfig> const	&server = parser.getServers();

		HttpRequest		request(str);

		request.print();
		HttpResponse	response(request, server);

		resString = response.generate();
	}
	catch (HttpException const &ex)
	{
		std::cerr << "Http error uccored: " << ex.what() << " with code: "
			<< ex.whatCode() << std::endl;
		resString = HttpResponse::createError(ex.whatCode(), parser.getServers().front());
	}
	catch (std::exception const &ex)
	{
		std::cerr << "Error occured: " << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Unkown throw ocurred" << std::endl;
	}
	std::cout << std::endl << "######Response:"
		<< std::endl << resString;
}
//		"User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:135.0)     \r\n"
//		"Cookie: _ga=GA1.2.549077782.1743611458; \r\n"
//		"Upgrade-Insecure-Requests: 1\r\n"
//		"Sec-Fetch-Dest: document\r\n"
//		"Sec-Fetch-Mode: navigate\r\n"
//		"Sec-Fetch-Site: none\r\n"
//		"Sec-Fetch-User: ?1\r\n"
*/
