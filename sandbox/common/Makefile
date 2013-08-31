all: config.o common.o

config.o: config.c config.h
	gcc -c config.c

common.o: common.c common.h
	gcc -c common.c

clean:
	-rm config.o common.o
