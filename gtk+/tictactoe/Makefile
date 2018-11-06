all: tictactoe

tictactoe: main.c gtktictactoe.c gtktictactoe.h gtktictactoe-marshal.c gtktictactoe-marshal.h Makefile
	gcc -o tictactoe -g main.c gtktictactoe.c gtktictactoe-marshal.c `pkg-config --libs --cflags gtk+-2.0`

gtktictactoe-marshal.c: gtktictactoe-marshal.list Makefile
	glib-genmarshal --body --prefix=gtk_tictactoe_marshal gtktictactoe-marshal.list > gtktictactoe-marshal.c

gtktictactoe-marshal.h: gtktictactoe-marshal.list Makefile
	glib-genmarshal --header --prefix=gtk_tictactoe_marshal gtktictactoe-marshal.list > gtktictactoe-marshal.h
