all: tictactoe

tictactoe: main.c gtktictactoe.c gtktictactoe.h
	gcc -o tictactoe -g main.c gtktictactoe.c `pkg-config --libs --cflags gtk+-2.0`
