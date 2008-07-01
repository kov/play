all: hello

hello: hello.c
	gcc -g -o hello hello.c `pkg-config --libs --cflags gtk+-2.0 clutter-0.6 clutter-gtk-0.6`
