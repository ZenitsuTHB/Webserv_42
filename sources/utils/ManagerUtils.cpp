/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManagerUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 14:57:54 by avolcy            #+#    #+#             */
/*   Updated: 2025/06/08 14:45:35 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes"

bool ServerManager::readClientData(int client_fd, std::string& clientBuffer) {
    char buffer[READ_BUFFER_SIZE];
    ssize_t bytes_read;

    if (clientBuffer.size() > MAX_TOTAL_REQUEST_SIZE) {
        sendErrorResponse(client_fd, 413);
        return false;
    }

    bytes_read = read(client_fd, buffer, sizeof(buffer));
    if (bytes_read <= 0) {
        if (bytes_read == 0 || errno != EWOULDBLOCK) {
            handleClientDisconnect(client_fd);
        }
        return false;
    }

    if (clientBuffer.size() + bytes_read > MAX_TOTAL_REQUEST_SIZE) {
        sendErrorResponse(client_fd, 413);
        return false;
    }

    clientBuffer.append(buffer, bytes_read);
    return true;
}