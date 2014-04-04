#ifndef _SRV_FRAMEWORK_H_
#define _SRV_FRAMEWORK_H_

#include "epoll_wrapper.h"
#include "host_addr.h"
#include <map>
#include <vector>

const int MAX_BUFFER_LEN = 4096;
const int MAX_CONN_NUM = 1024;

struct sock_context
{
    uint32_t id;
    host_addr addr;
    size_t recv_buf_size;
    size_t send_buf_size;
};

class udp_context
{
public:
    udp_context(): m_fd(-1), m_recv_buf(NULL) {}

    void create_buf(int buf_len = MAX_BUFFER_LEN)
    {
		m_recv_buf = new char[buf_len];
    }

    void release_buf()
    {
		if(m_recv_buf)
		{
		   delete m_recv_buf;
		   m_recv_buf = NULL;
		}
    }

public:
    int m_fd;
    sock_context m_conf;
    char *m_recv_buf;
};


class srv_framework
{
public:
    virtual ~srv_framework();

protected:
    typedef std::map<int, udp_context> udp_context_type;
    udp_context_type m_udp_context;

    typedef std::vector<host_addr> ha_list_type;
    bool initialize(const ha_list_type &ha_list, int max_conn_num = MAX_CONN_NUM);

    void _handle_epoll_();
/*
    void _handle_udp_packet_();
    virtual bool handle_udp_packet() = 0;
*/
   
private:
    epoll_wrapper m_epoll;
};

#endif

