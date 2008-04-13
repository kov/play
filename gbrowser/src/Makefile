all: gbrowser

gbrowser: main.c
	gcc -o gbrowser main.c `pkg-config --libs --cflags libglade-2.0 webkit-1.0`

clean:
	rm gbrowser
