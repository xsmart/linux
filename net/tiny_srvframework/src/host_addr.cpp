#include "host_addr.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

host_addr::host_addr(const std::string &ip, uint16_t port)
{
    set(ip, port);
}

void host_addr::set(const std::string &ip, uint16_t port)
{
    ::memset(&m_addr, 0, sizeof(sockaddr_in));
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    m_addr.sin_port = htons(port);
}

const sockaddr_in* host_addr::get_addr() const
{
    return &m_addr;
}

std::string host_addr::get_ip() const
{
    return std::string(inet_ntoa(m_addr.sin_addr));
}

uint16_t host_addr::get_port() const
{
    return ntohs(m_addr.sin_port);
}
