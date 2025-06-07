/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseReqUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 01:12:04 by avolcy            #+#    #+#             */
/*   Updated: 2025/06/07 02:31:35 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <cctype>
#include "../../includes/core/ParseReqUtils.hpp"

namespace parsereq 
{
    bool hasValidRequestLine(const std::string& request) {
        size_t firstCRLF = request.find("\r\n");
        if (firstCRLF == std::string::npos) return false;
        std::string startLine = request.substr(0, firstCRLF);
        return (startLine.find(" ") != std::string::npos);
    }
    
    bool isValidHttpVersion(const std::string& request) {
        size_t firstCRLF = request.find("\r\n");
        if (firstCRLF == std::string::npos) return false;
        std::string startLine = request.substr(0, firstCRLF);
        return (startLine.find("HTTP/1.1") != std::string::npos ||
                startLine.find("HTTP/1.0") != std::string::npos);
    }
    
    bool isEmptyRequest(const std::string& request) {
        if (request.empty()) return true;
    
        for (size_t i = 0; i < request.size(); ++i) {
            if (!isspace(static_cast<unsigned char>(request[i]))) {
                return false;
            }
        }
    
        return true;
    }
    
    std::string normalizeLineEndings(const std::string& str) {
        std::string result;
        for (size_t i = 0; i < str.size(); ++i) {
            if (str[i] == '\n') {
                if (i == 0 || str[i - 1] != '\r') {
                    result += "\r\n";
                } else {
                    result += '\n';
                }
            } else {
                result += str[i];
            }
        }
        return result;
    }
    
    std::string sanitizeRequest(const std::string& req) {
        std::string sanitized;
        for (size_t i = 0; i < req.size(); ++i) {
            char c = req[i];
            if ((c >= 32 && c <= 126) || c == '\t' || c == '\n' || c == '\r') {
                sanitized += c;
            }
        }
        return sanitized;
    }
    
    bool shouldCloseConnection(const std::string& headers) {
        size_t pos = headers.find("Connection:");
        if (pos == std::string::npos) return false;
        pos += 11;
        while (pos < headers.size() && isspace(headers[pos])) ++pos;
        std::string value = headers.substr(pos);
        size_t end = value.find("\r\n");
        if (end != std::string::npos) value = value.substr(0, end);
        return (value.find("close") != std::string::npos);
    }
    
    std::string prepareRequestForMax(const std::string& rawRequest, int client_fd) {
        (void)client_fd; // Suppress unused parameter warning
    
        if (isEmptyRequest(rawRequest)) {
            throw std::runtime_error("Empty request");
        }
    
        std::string normalized = normalizeLineEndings(rawRequest);
        std::string sanitized = sanitizeRequest(normalized);
    
        if (!hasValidRequestLine(sanitized)) {
            throw std::runtime_error("Invalid request line");
        }
    
        if (!isValidHttpVersion(sanitized)) {
            throw std::runtime_error("Unsupported HTTP version");
        }
    
        return sanitized;
    }
}
