#ifndef NETWORK_UTILS_HPP
#define NETWORK_UTILS_HPP

#include <string>
#include <sys/socket.h>

namespace NetworkUtils {
    void setNonBlocking(int fd, bool enable);
    void closeSocket(int fd);
    void epollControl(int epollFd, int op, int fd, uint32_t events);
    bool isConnectionClosed(const std::string& request);
}

#endif  