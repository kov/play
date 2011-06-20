all: test test-non-clutter

test: test.c
	gcc -o test test.c `pkg-config --cflags --libs webkit-1.0 clutter-gtk-0.90`

test-non-clutter: test-non-clutter.c
	gcc -o test-non-clutter test-non-clutter.c `pkg-config --cflags --libs webkit-1.0`
