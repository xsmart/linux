all: test_server test_client

test_server: test_server.cpp libtinysrv.a
	g++ test_server.cpp libtinysrv.a -I../include -o test_server

test_client: test_client.cpp libtinysrv.a
	g++ test_client.cpp libtinysrv.a -I../include -o test_client

clean:
	rm -f *.o test_server test_client
