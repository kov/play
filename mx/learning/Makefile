all: learning notebook

learning: learning.c Makefile
	gcc -g -O0 -o learning learning.c `pkg-config --cflags --libs mx-1.0 gio-2.0`

notebook: notebook.c Makefile
	gcc -g -O0 -o notebook notebook.c `pkg-config --cflags --libs mx-1.0 gio-2.0`
