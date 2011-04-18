all: test

test: Makefile test.c
	gcc -g -O0 -o test test.c `pkg-config --cflags --libs egl glesv2 glib-2.0 x11`
