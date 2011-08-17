all: gkcat

gkcat: gkcat.c Makefile
	gcc -g -std=c99 -o gkcat gkcat.c `pkg-config --cflags --libs gio-2.0`
