SUBDIRS = common apparmor ptrace
INCLUDE = common/*.h apparmor/helper.h ptrace/ptrace.h

define make_subdir
 	@for subdir in $(SUBDIRS) ; do \
 	(cd $$subdir && make $1) \
 	done;
endef

all: module sandbox.o sandbox

module: 
	$(call make_subdir , all)

sandbox.o: sandbox.c sandbox.h $(INCLUDE)
	gcc -c $< -I common -I apparmor -I ptrace

sandbox: main.c common/config.o common/common.o sandbox.o apparmor/libapparmor.so ptrace/libptrace.so
	gcc -o sandbox main.c common/config.o common/common.o sandbox.o -L. apparmor/libapparmor.so ptrace/libptrace.so -lpthread

clean:
	-rm sandbox.o
	$(call make_subdir , clean)
