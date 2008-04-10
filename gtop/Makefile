all: uptime

uptime: uptime.c
	gcc -g -o uptime uptime.c `pkg-config --cflags --libs libgtop-2.0`
