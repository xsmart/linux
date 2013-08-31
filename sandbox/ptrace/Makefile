INCLUDE = ../common/*.h
OBJECT = ptrace.o file_access.o reslimit.o

all: libptrace.so

libptrace.so: $(OBJECT)
	gcc -shared $(OBJECT) -o libptrace.so

ptrace.o: ptrace.c ptrace.h $(INCLUDE)
	gcc -c $< -I../common

file_access.o: file_access.c file_access.h $(INCLUDE)
	gcc -c $< -I../common

reslimit.o: reslimit.c reslimit.h $(INCLUDE)
	gcc -c  $< -I../common -lpthread

clean:
	-rm $(OBJECT)
