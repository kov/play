all: mjpeg-viewer

mjpeg-viewer: main.c Makefile
	gcc -g -std=c99 -o mjpeg-viewer main.c `pkg-config --libs --cflags gtk+-3.0 libsoup-2.4 gio-2.0`
