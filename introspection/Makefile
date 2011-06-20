all: test

test: test.c
	gcc -o test test.c `pkg-config --cflags --libs gobject-introspection-1.0 telepathy-glib`
