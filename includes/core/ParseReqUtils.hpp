/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseReqUtils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 00:48:54 by avolcy            #+#    #+#             */
/*   Updated: 2025/06/08 14:54:49 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_UTILS_HPP
#define SERVER_UTILS_HPP

#include <string>
#include <map>
#include <iostream>

// struct RequestData {
//     size_t headerEnd = 0;
//     size_t contentLength = 0;
//     std::string headers;
// };

namespace parsereq {

    // Request Validation
    bool hasValidRequestLine(const std::string& request);
    bool isValidHttpVersion(const std::string& version);
    bool isEmptyRequest(const std::string& request);
    bool hasValidHeaders(const std::string& headers);
    bool shouldCloseConnection(const std::string& headers);
    
    // Request Processing
    std::string sanitizeRequest(const std::string& req);
    std::string normalizeLineEndings(const std::string& str);
    std::string prepareRequestForMax(const std::string& rawRequest, int clientFd);
    
    // Header Utilities
    bool parseContentLength(const std::string& headers, size_t& outLength);
    std::string getHeaderValue(const std::string& headers, const std::string& key);
    bool hasHeader(const std::string& headers, const std::string& key);
    
    // Request Method Validation
    bool isSupportedMethod(const std::string& method);
    bool isRequestBodyValid(const std::string& method, const std::string& body);

} // namespace parsereq

#endif