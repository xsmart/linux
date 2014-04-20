#include "srv_framework.h"
#include <utility>
#include <iostream>

using std::cout;
using std::endl;

namespace tiny_srv{

srv_framework::~srv_framework()
{
	srv_framework::udp_context_type::iterator it;
	for(it = m_udp_context.begin(); it != m_udp_context.end(); it++)
	{
		it->second.release_buf();
		it->second.m_sock.close();
	}
}


bool srv_framework::initialize(const conf &srv_conf)
{
    if(!m_epoll.create(srv_conf.max_conn_num))
	{
		// need log
		cout << "srv_framework::initialize: m_epoll.create() failed" << endl;
		return false;
	}

	conf::ha_list_type::size_type sz  = srv_conf.ha_list.size();
	const conf::ha_list_type &ha_list = srv_conf.ha_list;
    for(uint32_t i = 0; i < sz; i++)
    {
		udp_context udp_if;
		udp_if.m_conf.id = i + 1;
		udp_if.m_conf.addr = ha_list[i];
		udp_if.m_conf.recv_buf_size = MAX_BUFFER_LEN;
		udp_if.m_conf.send_buf_size = 0;
		udp_if.create_buf(udp_if.m_conf.recv_buf_size);

		if(!udp_if.m_sock.create())
		{
			// need log
			cout << "srv_framework::initialize: create udp socket failed" << endl;
			return false;
		}
			
		if(!udp_if.m_sock.bind(ha_list[i]))
		{
			// need log
			cout << "srv_framework::initialize: create udp socket bind address failed" << endl;
			return false;
		}

		if(!m_epoll.add_fd(udp_if.m_sock.get_fd()))
		{
			// need log
			cout << "srv_framework::initialize: m_epoll.add_fd() failed" << endl;
			return false;
		}

		m_udp_context.insert(std::pair<int,udp_context>(udp_if.m_sock.get_fd(), udp_if));
    }

    return true;
}

void srv_framework::_handle_epoll_(int timeout)
{
    epoll_wrapper::result epoll_ret;

    epoll_ret = m_epoll.wait(timeout);
    
    if(epoll_ret.first)
    {
		for(uint32_t i = 0; i < epoll_ret.second; i++)
		{
			epoll_event &ev = epoll_ret.first[i];

			udp_context_type::iterator udp_it = m_udp_context.find(ev.data.fd);

			if(udp_it != m_udp_context.end())
			{
				_handle_udp_packet_(udp_it->second);
			}
		}
    }
}


bool srv_framework::_handle_udp_packet_(udp_context &udp_if)
{
	char 	  *buf    = udp_if.m_recv_buf;
	size_t 	  buf_len = udp_if.m_conf.recv_buf_size;
	host_addr &ha 	  = udp_if.m_conf.addr;

	// use edge-triggered behavior of epoll
	while(1)
	{
		ssize_t n = udp_if.m_sock.recvfrom(buf, buf_len, ha);
		if(n <= 0)
			return true;

		int sock_name = udp_if.m_conf.id;
		int sock_fd   = udp_if.m_sock.get_fd();
		return handle_udp_packet(sock_name, sock_fd, buf, n, ha);
	}
}


bool srv_framework::run(const conf &srv_conf)
{
	if(!initialize(srv_conf))
		return false;

	while(1)
	{
		_handle_epoll_(srv_conf.timeout);
		handle_loop();
	}

	return true;
}

} // end of namespace
