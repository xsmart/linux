#ifndef _SOCKET_H_
#define _SOCKET_H_

class host_addr;

class socket
{
public:
    socket();
    explicit socket(int fd);
    virtual ~socket();

    bool is_valid() const;
    bool bind(const host_addr &ha);
    bool connect(const host_addr &ha);
    bool set_nonblock();
	int get_fd() const;
    void close();

protected:
    int m_sockfd;
};

#endif
