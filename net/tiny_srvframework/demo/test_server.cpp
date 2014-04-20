#include "srv_framework.h"
#include "host_addr.h"
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

using namespace tiny_srv;

const int TIMEOUT = 2000;

class test_srv : public srv_framework
{
protected:
    bool handle_udp_packet(uint32_t sock_name, int sock_fd, const char* buf, size_t buf_len, const host_addr &ha)
	{
		string str(buf, buf_len);
		cout << str << endl;
		return true;
	}

	bool handle_loop()
	{
		cout << "calling handle_loop" << endl;
		return true;
	}

};

int main()
{
	test_srv srv;
	conf srv_conf;
	srv_conf.add_ha("192.168.1.106", 29873);
	srv.run(srv_conf);
    return 0;
}

