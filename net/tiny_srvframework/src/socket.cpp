#include "socket.h"
#include "host_addr.h"
#include <unistd.h>
#include <fcntl.h>

namespace tiny_srv{

socket::socket()
: m_sockfd(-1)
{}

socket::socket(int fd)
: m_sockfd(fd)
{}

socket::~socket()
{}

bool socket::is_valid() const
{
    return m_sockfd != -1;
}

bool socket::bind(const host_addr &ha)
{
    if(!is_valid())
		return false;

    return ::bind(m_sockfd, ha.get_addr(), sizeof(sockaddr)) == 0;
}

bool socket::connect(const host_addr &ha)
{
    if(!is_valid())
		return false;

    return ::connect(m_sockfd, ha.get_addr(), sizeof(sockaddr)) == 0;
}

bool socket::set_nonblock()
{
    if(!is_valid())
		return false;

    int flag = ::fcntl(m_sockfd, F_GETFL);
    if(flag != -1)
    {
    	return (::fcntl(m_sockfd, F_SETFL, flag | O_NONBLOCK) != -1);
    }

    return false;
}

int socket::get_fd() const
{
	return m_sockfd;
}

void socket::close()
{
    if(is_valid())
    {
		::close(m_sockfd);
		m_sockfd = -1;
    }
}

} // end of namespace
