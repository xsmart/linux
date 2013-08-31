INCLUDE = ../common/*.h
OBJECT = helper.o reslimit.o

all: libapparmor.so

libapparmor.so: $(OBJECT)
	gcc -shared helper.o reslimit.o -o libapparmor.so

helper.o: helper.c helper.h reslimit.h $(INCLUDE)
	gcc -c $< -I../common

reslimit.o: reslimit.c reslimit.h $(INCLUDE)
	gcc -c $< -I../common

clean:
	-rm $(OBJECT)
