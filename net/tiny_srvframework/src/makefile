INCLUDE = ../include

C_ARGS = -g -Wall
CXX = g++
CC  = gcc

all: socket.o host_addr.o udp_socket.o epoll_wrapper.o srv_framework.o tiny_srvframework 

socket.o: socket.cpp $(INCLUDE)/host_addr.h $(INCLUDE)/socket.h
	$(CXX) $(C_ARGS) -c socket.cpp -I$(INCLUDE) -o $@

udp_socket.o: udp_socket.cpp $(INCLUDE)/udp_socket.h $(INCLUDE)/socket.h $(INCLUDE)/host_addr.h
	$(CXX) $(C_ARGS) -c udp_socket.cpp -I$(INCLUDE) -o $@

host_addr.o: host_addr.cpp $(INCLUDE)/host_addr.h
	$(CXX) $(C_ARGS) -c host_addr.cpp -I$(INCLUDE) -o $@

epoll_wrapper.o: epoll_wrapper.cpp $(INCLUDE)/epoll_wrapper.h
	$(CXX) $(C_ARGS) -c epoll_wrapper.cpp -I$(INCLUDE) -o $@

srv_framework.o: srv_framework.cpp $(INCLUDE)/*.h
	$(CXX) $(C_ARGS) -c srv_framework.cpp -I$(INCLUDE) -o $@

tiny_srvframework: main.cpp $(INCLUDE)/epoll_wrapper.h
	$(CXX) $(C_ARGS) main.cpp host_addr.o epoll_wrapper.o srv_framework.o -I$(INCLUDE) -o $@

clean:
	rm -f *.o
