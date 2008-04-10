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
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <polkit-dbus/polkit-dbus.h>

void call_get(GError **error)
{
  DBusGConnection* bus = NULL;
  DBusGProxy *proxy = NULL;
  
  gdouble uptime;

  bus = dbus_g_bus_get(DBUS_BUS_SYSTEM, error);
  if(*error)
    {
      g_error((*error)->message);
      exit(1);
    }

  proxy = dbus_g_proxy_new_for_name(bus, "br.eti.kov.GUptime",
				    "/br/eti/kov/GUptime",
				    "br.eti.kov.GUptime");

  dbus_g_proxy_call(proxy, "get", error,
		    G_TYPE_INVALID,
		    G_TYPE_DOUBLE,
		    &uptime,
		    G_TYPE_INVALID);

  if(*error == NULL)
    g_print("uptime: %f\n", uptime);
  else
    g_warning("Failed to call 'get': %s\n", (*error)->message);
}

int main(int argc, char **argv)
{
  GError *error = NULL;

  g_type_init();

  call_get(&error);
  if(error)
    {
      gchar *error_message = g_strdup(error->message);

      g_error_free(error);
      error = NULL;

      if(g_str_has_prefix(error_message, "auth_"))
	{
	  DBusError dbus_error;
	  g_print("Trying to authenticate...\n");
	  dbus_error_init(&dbus_error);
	  polkit_auth_obtain("br.eti.kov.guptime.get",
			     0, getpid(), &dbus_error);
	  call_get(&error);
	}
    }

  return 0;
}
