all: test gio

test: test.c
	clang -fblocks -o test test.c -lBlocksRuntime

gio: gio.c
	clang -fblocks -o gio gio.c -lBlocksRuntime `pkg-config --cflags --libs gio-2.0`
