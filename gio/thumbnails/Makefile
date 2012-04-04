all: thumbnails

thumbnails: thumbnails.c Makefile
	gcc -o thumbnails thumbnails.c `pkg-config --cflags --libs gio-2.0`
