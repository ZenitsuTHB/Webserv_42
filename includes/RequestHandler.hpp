/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:27:43 by avolcy            #+#    #+#             */
/*   Updated: 2025/05/22 15:27:45 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <string>
#include <map>

class RequestHandler {
public:
    std::string readRequest(int fd, std::map<int, std::string>& buffers);
    std::string handleRequest(int fd, const std::string& rawRequest);
};

#endif
