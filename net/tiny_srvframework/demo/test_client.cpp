#include <iostream>
#include <string>
#include <unistd.h>

#include "udp_socket.h"
#include "host_addr.h"

using namespace tiny_srv;
using namespace std;

int main()
{
	udp_socket us;
	us.create();

	fork();

	host_addr ha("192.168.1.106", 51755);
	string str("test message");
	us.sendto(str.c_str(), str.length(), ha);
	cout << str << endl;
	return 0;
}
