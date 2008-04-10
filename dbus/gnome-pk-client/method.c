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
#include <gtk/gtk.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <polkit-dbus/polkit-dbus.h>
#include <polkit-gnome/polkit-gnome.h>

void call_get(void);

void get_activate_cb(PolKitGnomeAction *gnome_action, gpointer user_data)
{
  PolKitGnomeContext *pk_context;
  PolKitCaller *pk_caller;
  DBusError dbus_error;

  pk_context = polkit_gnome_context_get(NULL);

  dbus_error_init(&dbus_error);
  pk_caller = polkit_tracker_get_caller_from_pid (pk_context->pk_tracker, 
						  getpid (),
						  &dbus_error);
  if(dbus_error_is_set(&dbus_error))
    {
      g_error("error: %s: %s\n", dbus_error.name, dbus_error.message);
    }
  else
    {
      pid_t caller_pid;
      uid_t caller_uid;

      polkit_caller_get_pid(pk_caller, &caller_pid);
      polkit_caller_get_uid(pk_caller, &caller_uid);

      g_print("Caller from pid %d, uid %d\n", caller_pid, caller_uid);

      call_get();
    }
}

void auth_cb(PolKitAction *pk_action, gboolean gained_privilege,
	     GError *error, gpointer user_data)
{
  g_print("Gained privilege: %d\n", gained_privilege);
  if(error)
    {
      g_error("Error on auth_cb: %s\n", error->message);
    }
}

void call_get(void)
{
    DBusGConnection* bus = NULL;
    DBusGProxy *proxy = NULL;
    GError *error = NULL;

    gdouble uptime;

    bus = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error);
    if(error)
    {
        g_error(error->message);
        exit(1);
    }

    proxy = dbus_g_proxy_new_for_name(bus, "br.eti.kov.GUptime",
                                      "/br/eti/kov/GUptime",
                                      "br.eti.kov.GUptime");

    dbus_g_proxy_call(proxy, "get", &error,
                      G_TYPE_INVALID,
                      G_TYPE_DOUBLE,
                      &uptime,
                      G_TYPE_INVALID);
    g_print("uptime: %f\n", uptime);
}

int main(int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *button;

  PolKitGnomeContext* pk_context = NULL;
  PolKitAction *pk_action = NULL;
  PolKitGnomeAction *pk_gnome_action = NULL;

  GError *error = NULL;

  gtk_init(&argc, &argv);

  pk_context = polkit_gnome_context_get(&error);
  if(error)
    g_error("polkit_gnome_context failed: %s\n", error->message);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(G_OBJECT(window), "delete-event",
		   G_CALLBACK(gtk_main_quit), NULL);

  pk_action = polkit_action_new();
  polkit_action_set_action_id(pk_action, "br.eti.kov.guptime.get");
  pk_gnome_action = polkit_gnome_action_new_default("get", pk_action,
						    "Uptime!", NULL);

  g_signal_connect(G_OBJECT(pk_gnome_action), "activate",
		   G_CALLBACK(get_activate_cb), NULL);

  button = polkit_gnome_action_create_button(pk_gnome_action);
  gtk_container_add(GTK_CONTAINER(window), button);

  gtk_widget_show_all(window);

  /* polkit_gnome_auth_obtain(pk_action, 0, getpid(), auth_cb, NULL, &error); */
  /* if(error) */
  /*   { */
  /*     g_error("Failed to auth: %s\n", error->message); */
  /*   } */

  gtk_main();

  polkit_action_unref(pk_action);
  return 0;
}

