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
#include <sstream>
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
		return (conf.getLocation(path));
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
	_version("HTTP/1.1"), _serverConf(conf), _index(false)
{
	std::cout << "This is length, this is the end: " << req.getBody().length() << std::endl;
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
	struct stat	sb;
	int			ret;

	if (stat(file.c_str(), &sb) == -1)
	{
		return -1;
		switch (errno)
		{
			case ENOTDIR:
				std::cout << "2" << std::endl;
				return (ENOTDIR);
			case ENOENT:
				std::cout << "3" << std::endl;
				return (ENOENT);
			case EACCES:
				std::cout << "4" << std::endl;
				return (EACCES);
			case ENAMETOOLONG:
				std::cout << "5" << std::endl;
				return (ENAMETOOLONG);
			case ELOOP:
				std::cout << "6" << std::endl;
				return (ELOOP);
			default:
				std::cout << "7" << std::endl;
				return (0);
		}
	}
	ret = 0;
	std::cout << "st_mode & S_IXUSR " << sb.st_mode << " & " << S_IXUSR
		<< " = " << (sb.st_mode & S_IXUSR) << std::endl;
	if (S_ISREG(sb.st_mode))
	{
				std::cout << "8" << std::endl;
		return (sb.st_mode);
	}
	else if (S_ISDIR(sb.st_mode))
	{
				std::cout << "9" << std::endl;
		return (sb.st_mode);
	}
	return (0);
}

void	HttpResponse::getCgi(HttpRequest const &request)
{
	std::string	fileName(request.getPath());
	int			fileStat;

	if (!isCgiAllowed(fileName, _serverConf))
		throw (HttpException("getCGI not allowed here", 403));
	fileName.insert(0, _route->getRoot());
	std::cout << "Looking for this file: " << fileName << std::endl;
	fileStat = checkFile(fileName);
	std::cout << "file stat: " << fileStat << std::endl;
	if (!S_ISREG(fileStat))
		throw (HttpException("getCGI file not found", 404));
	else if (!(fileStat & S_IXUSR))
		throw (HttpException("getCGI file x deniedn", 404));
	_body = executeGetCgi(fileName, request);
	Headers::const_iterator it = request.getHeaders().find("Content-type");
	if (it != request.getHeaders().end())
		_header.insert(PairStr("Content-type", it->second));
	else
		_header.insert(PairStr("Content-type", "text/plain"));
	_header.insert(PairStr("Content-length",
				Libft::itos(static_cast<int>(_body.length()))));
	_code = 200;
	//throw (HttpException("getCGI Not done yet", 500));
}

void	HttpResponse::postCgi(HttpRequest const &request)
{
	std::string	output;
	std::string	filename(request.getPath());

	/*
	try
	{
	*/
		if (!isCgiAllowed(filename, _serverConf))
			throw (HttpException("getCGI not allowed here", 403));
		output = executePostCgi(filename, request);
	/*
		std::cout << "Se ha executado efectivamente" << std::endl;
	}
	catch (std::exception const &e)
	{
		output.clear();
		std::cerr << "CGI execution failed: " << e.what() << std::endl;
	}
	*/

	_body = output;
	std::string len = Libft::itos(static_cast<int>(output.length()));
	_header.insert(make_pair("Content-length", len));
	_code = 200;
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

	if (isCgi(fileName))
		return (postCgi(request));

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
	else if (file.find(".js") != std::string::npos)
		type = Http::getFileType("js");
	else if (file.find(".json") != std::string::npos)
		type = Http::getFileType("json");
	else if (file.find(".xml") != std::string::npos)
		type = Http::getFileType("xml");
	else if (file.find(".pdf") != std::string::npos)
		type = Http::getFileType("pdf");
	else if (file.find(".zip") != std::string::npos)
		type = Http::getFileType("zip");
	else if (file.find(".png") != std::string::npos)
		type = Http::getFileType("png");
	else if (file.find(".jpeg") != std::string::npos)
		type = Http::getFileType("jpeg");
	else if (file.find(".jpg") != std::string::npos)
		type = Http::getFileType("jpg");
	else if (file.find(".gif") != std::string::npos)
		type = Http::getFileType("gif");
	else if (file.find(".webp") != std::string::npos)
		type = Http::getFileType("webp");
	else if (file.find(".mpeg") != std::string::npos)
		type = Http::getFileType("mpeg");
	else if (file.find(".ogg") != std::string::npos)
		type = Http::getFileType("ogg");
	else if (file.find(".mp4") != std::string::npos)
		type = Http::getFileType("mp4");
	else if (file.find(".webm") != std::string::npos)
		type = Http::getFileType("webm");
	else if (file.find(".ogg") != std::string::npos)
		type = Http::getFileType("ogg");
	else
		type.assign("text/plain");
	return (type);
}
std::string	HttpResponse::_indexFolder(std::string &folder)
{
	std::string	index;
	std::string	fileLine;

	index = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n"
		"  <meta charset=\"UTF-8\">\n  <title>Index of /my-folder</title>\n"
		"</head>\n<body>\n  <h1>Index of /my-folder</h1>\n  <ul>\n"
		"  </ul>\n</body>\n</html>";
	folder = "index.html";
	fileLine = "    <li><a href=\"file\">file</a></li>\n";
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
	if (!page.empty())
	{
	std::cerr << "Im reading this: " << page << std::endl;
	try
	{
		struct stat	sb;
		if (stat(page.c_str(), &sb) == -1 || !S_ISREG(sb.st_mode))
			throw (std::invalid_argument("getHedBody Page not found"));
		body = Libft::readFile(page);
		header.insert(PairStr("Content-type", HttpResponse::_fileType(page)));
	}
	catch (std::exception const &ex)
	{
		body.clear();
		header.insert(PairStr("Content-type", "text/plain"));
		std::cerr << "Failed to get error Page" << ex.what() << std::endl;
	}
	}
	//header.insert(PairStr("Content-type", HttpResponse::_fileType(page)));
	//Now is done inside try
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
	RouteConfig const	*route;

	route = server.getLocation(command);
	if (!route->isCgiEnabled())
	{
		std::cout << "Adios not enable: " << route->getPath() << std::endl;
		return false;
	}
	
	std::cout << "Hola Mondo" << std::endl;
	VectorStr const ext = route->getCgiExtensions();
	
	/*
	for (size_t i = 0; i < ext.size(); i++)
	{
		int	len = command.length() - ext[i].length();
		if (len >= 0 && ext[i] == command.substr(len))
			return (true);
	}*/
	return true;
}

std::string HttpResponse::executeGetCgi(std::string const &command, HttpRequest const &request)
{
	int	pipefd[2];
	std::string	cgiOut;

	if (pipe(pipefd) == -1)
	{
		std::cerr << "Pipe error: " << strerror(errno) << std::endl;
		throw (HttpException("get CGI pipe error", 500));
	}

	pid_t	pid = fork();
	if (pid < 0)
	{
		std::cerr << "Fork failed: " << strerror(errno) << std::endl;
		close(pipefd[0]);
		close(pipefd[1]);
		throw (HttpException("get CGI fork error", 500));
	}

	if (pid == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);

		char	*argv[] = { const_cast<char*>(command.c_str()), NULL };
		std::string	scritFile = "SCRIPT_FILENAME=" + command;
		std::string	queryStr = "QUERY_STRING=" + request.getQuery();
		char	*envp[] = {
			const_cast<char*>("REQUEST_METHOD=GET"),
			const_cast<char*>("GATEWAY_INTERFACE=CGI/1.1"),
			const_cast<char*>(scritFile.c_str()),
			const_cast<char*>(queryStr.c_str()),
			NULL
		};

		execve(command.c_str(), argv, envp);
		std::cerr << "get CGI failed execve" << strerror(errno) << std::endl;
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
	{
		std::cout << "this is da return of program: " << ret << std::endl;
		throw (HttpException("get CGI failed program status code", 500));
	}
	return (cgiOut);
}

std::string HttpResponse::executePostCgi(std::string const &command, HttpRequest const &request)
{
	int				stdoutPipe[2];
	int				stdinPipe[2];
	std::string		output;

	if (pipe(stdoutPipe) == -1 || pipe(stdinPipe) == -1)
		throw HttpException("CGI post pipe error", 500);

	pid_t	pid = fork();
	if (pid < 0)
	{
		close(stdoutPipe[0]); close(stdoutPipe[1]);
		close(stdinPipe[0]); close(stdinPipe[1]);
		throw HttpException("CGI post fork error", 500);
	}
	if (pid == 0)
	{
		dup2(stdoutPipe[1], STDOUT_FILENO);
		dup2(stdinPipe[0], STDIN_FILENO);

		close(stdoutPipe[0]); close(stdoutPipe[1]);
		close(stdinPipe[0]); close(stdinPipe[1]);
		
		//Como vas a convertir un string a un numero asi
		std::ostringstream	lengthStream;
		lengthStream << request.getBody().length();

		std::string	scriptFile = "SCRIPT_FILENAME=" + command;
		std::string	requestMethod = "REQUEST_METHOD=POST";
		std::string	contentLength = "CONTENT_LENGTH=" + lengthStream.str();

		std::string	contentType = "CONTENT_TYPE=application/x-www-form-urlencoded";
		std::string	queryString = "QUERY_STRING=" + request.getQuery();

		char	*argv[] = { const_cast<char *>(command.c_str()), NULL };
		char	*envp[] = {
				const_cast<char *>("GATEWAY_INTERFACE-CGI/1.1"),
				const_cast<char *>(requestMethod.c_str()),
				const_cast<char *>(scriptFile.c_str()),
				const_cast<char *>(contentLength.c_str()),
				const_cast<char *>(contentType.c_str()),
				const_cast<char *>(queryString.c_str()),
				NULL
		};

		execve(command.c_str(), argv, envp);
		std::cout << "post CGI failed execve" << strerror(errno) << std::endl;
		std::string	err(strerror(errno));
		exit (127);
	}
	
	close(stdoutPipe[1]); close(stdinPipe[0]);

	std::string const	&body = request.getBody();
	write(stdinPipe[1], body.c_str(), body.length());
	close(stdinPipe[1]);

	char	buffer[1024];
	ssize_t	count;

	while ((count = read(stdoutPipe[0], buffer, sizeof(buffer))) > 0)
		output.append(buffer, count);
	close(stdoutPipe[0]);

	int status = 0;
	
	waitpid(pid, &status, 0);
	
	if (status != 0)
		throw HttpException("CGI post failed program", 500);

	return output;
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

/*
#include <iostream>
#include <cstdlib>
#include "../../includes/ServerConfig.hpp"
#include "../../includes/ParserConfig.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	std::string	str =
		"GET /cgi-bin/hola.out HTTP/3\r\n"
		"Host: http.cat\r\n"
		"Content-type: plain/text\r\n"
		"Accept: text/html,application/xhtml+xml,application/xml;\r\n"
		"Accept-Language: en-US,en;q=0.5\r\n"
		"Accept-Encoding: gzip, deflate, br, zstd\r\n"
		"Connection: keep-alive\r\n"
		"\t\v\r\f Priority\t\v\r\f :\t\v\r\f u=0, i\t\v\r\f \r\n\r\n"
		"12345678901234567890123456789012345678901234567890\nHolaMundo\n";

		std::string	resString;
		ParserConfig					parser(argv[1]);
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
