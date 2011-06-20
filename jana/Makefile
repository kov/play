all: test

test: test.c Makefile
	gcc -g -O0 -o test test.c `pkg-config --cflags --libs libjana-ecal`
