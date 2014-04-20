#include "conf.h"

namespace tiny_srv{

conf::conf()
: max_conn_num(MAX_CONN_NUM), timeout(BLOCK)
{}


void conf::add_ha(const host_addr &ha)
{
	ha_list.push_back(ha);
}

void conf::add_ha(const std::string &ip, uint16_t port)
{
	ha_list.push_back(host_addr(ip, port));
}

} // end of namespace
