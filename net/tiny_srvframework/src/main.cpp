#include "srv_framework.h"
#include "host_addr.h"
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

class test_srv : public srv_framework
{
protected:
    bool handle_udp_packet(uint32_t sock_name, int sock_fd, const char* buf, size_t buf_len, const host_addr &ha)
	{
		string str(buf, buf_len);
		cout << str << endl;
		return true;
	}

};

int main()
{
	test_srv srv;
	ha_list_type ha_list;
	ha_list.push_back(host_addr("192.168.1.106", 51755));
	ha_list.push_back(host_addr("192.168.1.106", 29873));
	srv.run(ha_list);
    return 0;
}
