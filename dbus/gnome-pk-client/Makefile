all: method

method: method.c
	gcc -g -o method method.c `pkg-config --cflags --libs gtk+-2.0 dbus-glib-1 polkit-dbus polkit-gnome`
