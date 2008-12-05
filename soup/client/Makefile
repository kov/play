all: client

client: main.c
	gcc -g -o client main.c `pkg-config --cflags --libs libsoup-2.4`
