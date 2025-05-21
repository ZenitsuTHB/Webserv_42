/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:13:08 by avolcy            #+#    #+#             */
/*   Updated: 2025/05/21 18:39:26 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_MANAGER_HPP
#define CLIENT_MANAGER_HPP

#include "BaseSocket.hpp"
#include <map>
#include <string>

class ClientManager {
public:
    void addClient(int fd, const BaseSocket& socket);
    void removeClient(int fd, int epollFd);
    bool clientExists(int fd) const;
    void updateClientBuffer(int fd, const std::string& data);
    void clearClientBuffer(int fd);
    const std::string& getClientBuffer(int fd) const;
    const std::map<int, BaseSocket>& getClients() const;
    std::map<int, std::string>& getClientBuffers();


private:
    std::map<int, BaseSocket>  _clients;
    std::map<int, std::string> _clientBuffers;
};

#endif