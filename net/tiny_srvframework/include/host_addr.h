#ifndef _HOST_ADDR_H_
#define _HOST_ADDR_H_

#include <string>
#include <netinet/in.h>

class host_addr
{
public:
    host_addr(const std::string &ip, uint16_t port);
    void set(const std::string &ip, uint16_t port);
    
    const sockaddr_in* get_addr() const;
    std::string get_ip() const;
    uint16_t get_port() const;

private:
    sockaddr_in m_addr;     
};

#endif
