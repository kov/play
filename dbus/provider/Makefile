all: provider

clean:
	rm provider

provider: provider.c guptime.c guptime-server-bindings.h
	gcc -g -o provider provider.c guptime.c `pkg-config --cflags --libs dbus-glib-1 libgtop-2.0 polkit-dbus polkit`

guptime-server-bindings.h: dbus-guptime.xml
	dbus-binding-tool --prefix=g_uptime --mode=glib-server $<> $@
