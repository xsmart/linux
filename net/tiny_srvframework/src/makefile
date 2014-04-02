INCLUDE = ../include

C_ARGS = -g -Wall
CXX = g++
CC  = gcc

all: host_addr.o tiny_srvframework

host_addr.o: host_addr.cpp $(INCLUDE)/host_addr.h
	$(CXX) $(C_ARGS) -c host_addr.cpp -I$(INCLUDE) -o $@

tiny_srvframework: main.cpp epoll_wrapper.cpp $(INCLUDE)
	$(CXX) $(C_ARGS) main.cpp epoll_wrapper.cpp -I$(INCLUDE) -o $@

clean:
	rm -f *.o
