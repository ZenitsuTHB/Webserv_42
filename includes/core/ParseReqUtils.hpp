/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseReqUtils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 00:48:54 by avolcy            #+#    #+#             */
/*   Updated: 2025/06/07 02:27:14 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_UTILS_HPP
#define SERVER_UTILS_HPP

#include <string>
#include <iostream>

namespace parsereq {

bool        hasValidRequestLine(const std::string& request);
bool        isValidHttpVersion(const std::string& request);
bool        isEmptyRequest(const std::string& request);
std::string sanitizeRequest(const std::string& req);
std::string normalizeLineEndings(const std::string& str);
bool        shouldCloseConnection(const std::string& headers);
std::string prepareRequestForMax(const std::string& rawRequest, int clientFd);


} // namespace http

#endif