#include "srv_framework.h"

srv_framework::~srv_framework()
{}


bool srv_framework::initialize(const srv_framework::ha_list_type &ha_list, int max_conn_num)
{
    m_epoll.create(max_conn_num);

    srv_framework::ha_list_type::size_type sz = ha_list.size();
    for(uint32_t i = 0; i < sz; i++)
    {
		udp_context udp_if;
		udp_if.m_conf.id = i + 1;
		udp_if.m_conf.addr = ha_list[i];
		udp_if.m_conf.recv_buf_size = MAX_BUFFER_LEN;
		udp_if.m_conf.send_buf_size = 0;
    }

    return true;
}

void srv_framework::_handle_epoll_()
{
    epoll_wrapper::result epoll_ret;

    epoll_ret = m_epoll.wait();
    
    if(epoll_ret.first)
    {
		for(uint32_t i = 0; i < epoll_ret.second; i++)
		{
			epoll_event &ev = epoll_ret.first[i];
		}
    }
}

