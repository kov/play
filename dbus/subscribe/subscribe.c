/*
 * Copyright (C) 2008 Gustavo Noronha Silva
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.  You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdlib.h>
#include <glib.h>
#include <dbus/dbus-glib.h>

void elapsed_changed_cb(DBusGProxy *object, guint elapsed, gpointer user_data)
{
    g_print("Elapsed: %d\n", elapsed);
}

int main(int argc, char **argv)
{
    DBusGConnection* bus = NULL;
    DBusGProxy *proxy = NULL;
    GError *error = NULL;
    GMainLoop *main = NULL;

    g_type_init();
    bus = dbus_g_bus_get(DBUS_BUS_SESSION, &error);
    if(error)
    {
        g_error(error->message);
        exit(1);
    }

    proxy = dbus_g_proxy_new_for_name(bus, "org.gnome.Rhythmbox",
                                      "/org/gnome/Rhythmbox/Player",
                                      "org.gnome.Rhythmbox.Player");
    dbus_g_proxy_add_signal(proxy, "elapsedChanged",
                            G_TYPE_UINT, G_TYPE_INVALID);
    dbus_g_proxy_connect_signal(proxy, "elapsedChanged",
                                G_CALLBACK(elapsed_changed_cb), NULL, NULL);

    main = g_main_loop_new(NULL, TRUE);
    g_main_loop_run(main);

    return 0;
}

