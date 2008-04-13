all: prog

prog: libbleh.so prog.c
	gcc -o prog prog.c -lbleh -L`pwd`

libbleh.so: lib.c lib.h
	gcc -o libbleh.so -shared lib.c
