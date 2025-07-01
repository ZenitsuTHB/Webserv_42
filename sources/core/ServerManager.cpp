/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*  ServerManager.cpp                                    :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:22:24 by avolcy            #+#    #+#             */
/*  Updated: 2025/06/28 19:19:11 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <map>
#include "../../includes/core/ServerManager.hpp"
#include "../../includes/HttpRequest.hpp"
#include "../../includes/HttpResponse.hpp"
#include "../../includes/exceptions.hpp" 

ServerManager::ServerManager(const std::vector<ServerConfig>& configs) : _epoll_fd(-1), _running(false) {
    _epoll_fd = epoll_create1(0);
    if (_epoll_fd == -1) {
        throw std::runtime_error(std::string("epoll_create1: ") + strerror(errno));
    }

    // Reserve space to prevent reallocations (minor optimization)
    _servers.reserve(configs.size());
    std::vector<ServerConfig>::const_iterator it;
    try {
        for (it = configs.begin(); it != configs.end(); ++it) {
            SmartPtr<Server> server(new Server(it->getIp(), it->getPort(), it->getBacklog(), _epoll_fd, *it));
            
            if (server->getSocketFd() == -1) { continue; } // Automated deletion when SmartPtr is out of scope
            
                addToEpoll(server->getSocketFd(), EPOLLIN);
                _fd_to_server[server->getSocketFd()] = server.getPtr();
                _servers.push_back(server.release()); // Ownership transferred
        }           
    } catch (...) {
        // Cleanup any successfully created servers if we fail mid-construction
        stop();
        throw;
    }
    _running = (!_servers.empty());
}

void ServerManager::stop() {
    if (!_running) return;
    for (std::vector<Server*>::iterator it = _servers.begin(); 
         it != _servers.end(); ++it) {
        delete *it;
    }
    _servers.clear();
    _fd_to_server.clear();
    _client_to_server.clear();
    _buffers.clear();
    _writeBuffers.clear();

    if (_epoll_fd != -1) {
        close(_epoll_fd);
        _epoll_fd = -1;
    }
    _running = false;
}

void ServerManager::run() {
       
    struct epoll_event events[MAX_EVENTS];
    while (_running) {
        
        if (g_shouldStop) {
            std::cout << "\n[SIGNALS] Caught termination signal. Stopping server..." << std::endl;
            stop();
            break ;
        }
        
        time_t now = time(NULL);
        for (TimeMap::iterator it = _lastActivity.begin(); it != _lastActivity.end(); ) {
            if (now - it->second > 42) {
                std::cout << "[TIMEOUT] Closing idle client: " << it->first << std::endl;
                handleClientDisconnect(it->first);
                _lastActivity.erase(it++);
            } else {
                ++it;
            }
        }

        int nfds = epoll_wait(_epoll_fd, events, MAX_EVENTS, 100);
        if (nfds == -1) {
            if (errno == EINTR) continue;
            throw std::runtime_error(std::string("epoll_wait: ") + strerror(errno));
        }

        for (int i = 0; i < nfds; ++i) {
            int fd = events[i].data.fd;

            if (_client_to_server.find(fd) != _client_to_server.end()) {
                _lastActivity[fd] = now;
                if (events[i].events & EPOLLOUT) {
                    flushWriteBuffer(fd);
                } else {
                    handleClientData(fd, events[i].events);
                }
            } else if (_fd_to_server.find(fd) != _fd_to_server.end()) {
                handleNewConnection(_fd_to_server[fd]);
            } else {
                std::cerr << "[ServerManager] Unknown fd triggered epoll: " << fd << std::endl;
            }
            if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                handleClientDisconnect(fd);
            }
        }
    }
}

void ServerManager::handleNewConnection(Server* server) {
    int client_fd = server->acceptConnection();
    if (client_fd == -1) return;

    _client_to_server[client_fd] = server;
    addToEpoll(client_fd, EPOLLIN | EPOLLET | EPOLLRDHUP);
}

bool ServerManager::parseContentLength(const std::string& headers, size_t& outLength) {
	size_t clPos = headers.find("Content-Length:");
	if (clPos == std::string::npos)
		return false;

	clPos += 15; // skip label
	while (clPos < headers.size() && isspace(headers[clPos])) ++clPos;

	std::string lenStr;
	while (clPos < headers.size() && isdigit(headers[clPos])) {
		lenStr += headers[clPos];
		++clPos;
	}
	if (lenStr.empty())
		return false;

    std::istringstream iss(lenStr);
    unsigned long len = 0;
    if (!(iss >> len)) return false;
    outLength = static_cast<size_t>(len);  //avoid portability issues on 32-bit sys  
	return true;
}

std::string    responseMessage(std::string const &request, ServerConfig const &server)
{
    std::string response;

    try
    {
        HttpRequest requestObj(request);
		std::cout << "###################Request###########" << std::endl;
		requestObj.print();
		std::cout << "###################Response###########" << std::endl;
        HttpResponse responseObj(requestObj, server);

        response = responseObj.generate();
		std::cout << response << std::endl << std::endl;
    }
    catch (const HttpException & ex)
    {
		std::cerr << "HttpException launched for: " << ex.what() << std::endl;
        response = HttpResponse::createError(ex.whatCode(), server);
    }
    catch(const std::exception& e)
    {
        std::cerr << "std::exception launched: " << e.what() << std::endl;
        response = HttpResponse::createError(500, server);
    }
    return (response);
}
    
void ServerManager::handleClientData(int client_fd, uint32_t events) {
    (void)events;
    Server* server = _client_to_server[client_fd];

    std::string& clientBuffer = _buffers[client_fd];

    if (clientBuffer.size() > MAX_TOTAL_REQUEST_SIZE) {
        std::cerr << "[ServerManager] Request too large from client: " << client_fd << std::endl;
        handleClientDisconnect(client_fd);
        return;
    }

    char buffer[READ_BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(client_fd, buffer, sizeof(buffer)))) {
        if (bytes_read <= 0) break;

        if (clientBuffer.size() + bytes_read > MAX_TOTAL_REQUEST_SIZE) {
            std::cerr << "[ServerManager] Request size limit exceeded from client: "
                      << client_fd << std::endl;
            handleClientDisconnect(client_fd);
            return;
        }

        clientBuffer.append(buffer, bytes_read);

        size_t headerEnd = clientBuffer.find("\r\n\r\n");
        if (headerEnd == std::string::npos)
            continue;

        std::string headers = clientBuffer.substr(0, headerEnd);
        size_t contentLength = 0;
        bool hasContentLength = headers.find("Content-Length:") != std::string::npos;

        if (hasContentLength) {
            if (!parseContentLength(headers, contentLength)) {
                std::cerr << "[ServerManager] Malformed Content-Length header from client: "
                          << client_fd << std::endl;
                handleClientDisconnect(client_fd);
                return;
            }

            if (contentLength > MAX_BODY_SIZE) {
                std::cerr << "[ServerManager] Content-Length too large from client: "
                          << client_fd << std::endl;
                handleClientDisconnect(client_fd);
                return;
            }

            size_t bodySize = clientBuffer.size() - (headerEnd + 4);
            if (bodySize < contentLength)
                continue;
        }
        try {
            std::string rawRequest = clientBuffer.substr(0, headerEnd + 4 + contentLength);
            
            //Prepare cleaned HTTP string
            std::string cleaned = parsereq::prepareRequestForMax(rawRequest, client_fd);
            std::string fullResponse = responseMessage(cleaned, server->getConfig());
            _writeBuffers[client_fd] = fullResponse;
            modifyEpoll(client_fd, EPOLLOUT | EPOLLET | EPOLLRDHUP);
            clientBuffer.erase(0, headerEnd + 4 + contentLength);
            //flushWriteBuffer(client_fd);
            if (parsereq::shouldCloseConnection(cleaned)) {
                _pendingClose[client_fd] = true;
            }
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Failed to prepare request for HttpRequest: " << e.what() << std::endl;
            handleClientDisconnect(client_fd);
        }

        break;
    }

    if (clientBuffer.size() > MAX_HEADER_SIZE &&
        clientBuffer.find("\r\n\r\n") == std::string::npos) {
        std::cerr << "[ServerManager] Header too large from client: " << client_fd << std::endl;
        handleClientDisconnect(client_fd);
    }
}

void ServerManager::flushWriteBuffer(int client_fd) {
    std::map<int, std::string>::iterator it = _writeBuffers.find(client_fd);
    if (it == _writeBuffers.end()) return;

    const std::string& response = it->second;
    ssize_t sent = send(client_fd, response.c_str(), response.size(), MSG_NOSIGNAL);

    if (sent == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            handleClientDisconnect(client_fd);
        }
        return;
    }

    if ((size_t)sent < response.size()) {
        // Partial write
        _writeBuffers[client_fd] = response.substr(sent);
        return;
    }

    // Full response sent
    //shutdown(client_fd, SHUT_WR) this signal EOF, tells the kernel to flush any buffered 
    //output and sends a FIN packet to the client, indicating that the server is done writing.
    
    _writeBuffers.erase(client_fd);
    shutdown(client_fd, SHUT_WR);//tells the kernel to flush any buffered output and sends a FIN packet to the client, indicating that the server is done writing.
    _buffers.erase(client_fd);  // 🧼 Clean up input buffer too

    if (_pendingClose.count(client_fd)) {
        _pendingClose.erase(client_fd);
        handleClientDisconnect(client_fd); // 🔥 Respect Connection: close
    } else {
        modifyEpoll(client_fd, EPOLLIN | EPOLLET | EPOLLRDHUP);
    }
}


void ServerManager::handleClientDisconnect(int client_fd) {
    removeFromEpoll(client_fd);
    _client_to_server.erase(client_fd);
    _buffers.erase(client_fd);
    _writeBuffers.erase(client_fd);
    close(client_fd);
}

void ServerManager::addToEpoll(int fd, uint32_t events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        throw std::runtime_error(std::string("epoll_ctl ADD: ") + strerror(errno));
    }
}

void ServerManager::modifyEpoll(int fd, uint32_t events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, fd, &ev) == -1) {
        std::cerr << "Warning: epoll_ctl MOD failed: " << strerror(errno) << std::endl;
    }
}

void ServerManager::removeFromEpoll(int fd) {
    if (fcntl(fd, F_GETFD) == -1 && errno == EBADF) return; // Already closed
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        std::cerr << "Warning: epoll_ctl DEL failed: " << strerror(errno) << std::endl;
    }
}


ServerManager::~ServerManager() {
    stop();
}
