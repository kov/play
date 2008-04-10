all: method

method: method.c
	gcc -g -o method method.c `pkg-config --cflags --libs dbus-glib-1 polkit-dbus`

clean:
	rm method