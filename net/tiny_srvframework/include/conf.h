#ifndef _CONF_H_
#define _CONF_H_

#include "host_addr.h"
#include <vector>

namespace tiny_srv{

enum{
	BLOCK  = -1,
	NOWAIT = 0
};

const int MAX_CONN_NUM = 1024;

class conf
{
public:
	conf();
	void add_ha(const host_addr &ha);
	void add_ha(const std::string &ip, uint16_t port);

	typedef std::vector<host_addr> ha_list_type;
	ha_list_type ha_list;
	int max_conn_num;
	int timeout;
};

} // end of namespace
#endif
