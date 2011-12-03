all: gkget

gkget: gkget.c Makefile
	gcc -g -std=c99 -o gkget gkget.c `pkg-config --cflags --libs gio-2.0 libsoup-2.4`

clean:
	rm gkget