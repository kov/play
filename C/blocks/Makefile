all: test gio gkget

test: test.c Makefile
	clang -fblocks -o test test.c -lBlocksRuntime

gio: gio.c Makefile
	clang -fblocks -o gio gio.c -lBlocksRuntime `pkg-config --cflags --libs gio-2.0`

gkget: gkget.c Makefile
	clang -fblocks -o gkget gkget.c -lBlocksRuntime `pkg-config --cflags --libs gio-2.0 libsoup-2.4`

clean:
	rm test gio gkget