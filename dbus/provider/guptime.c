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
#include <glib-object.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <dbus/dbus.h>
#include <polkit-dbus/polkit-dbus.h>
#include <glibtop/uptime.h>

#include "guptime.h"
#include "guptime-server-bindings.h"

static void g_uptime_init(GUptime *self);
static void g_uptime_class_init(GUptimeClass *klass);
DBusHandlerResult g_uptime_handle_dbus_message(DBusConnection *conn,
                                               DBusMessage *message,
                                               void *user_data);

GType g_uptime_get_type(void)
{
  static GType type = 0;

  if(type == 0)
    {
      type = g_type_register_static_simple(G_TYPE_OBJECT,
					   "GUptimeType",
					   sizeof(GUptimeClass),
					   (GClassInitFunc)g_uptime_class_init,
					   sizeof(GUptime),
					   (GInstanceInitFunc)g_uptime_init,
					   0);
    }
  return type;
}

static void g_uptime_class_init(GUptimeClass *klass)
{
  PolKitContext *pk_context = NULL;
  PolKitTracker *pk_tracker = NULL;
  DBusConnection *dbus_con = NULL;
  GError *error = NULL;
  PolKitError *pk_error = NULL;

  klass->dbus = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error);
  if(error)
    {
      g_error(error->message);
      exit(1);
    }
  dbus_con = dbus_g_connection_get_connection(klass->dbus);

  pk_context = polkit_context_new();
  polkit_context_init(pk_context, &pk_error);
  if(polkit_error_is_set(pk_error))
    {
      g_error("polkit_context_init failed: %s: %s\n",
	      polkit_error_get_error_name(pk_error),
	      polkit_error_get_error_message(pk_error));
    }
  klass->pk_context = pk_context;

  pk_tracker = polkit_tracker_new();
  polkit_tracker_set_system_bus_connection(pk_tracker, dbus_con);
  polkit_tracker_init(pk_tracker);
  klass->pk_tracker = pk_tracker;

  dbus_g_object_type_install_info(G_TYPE_UPTIME,
				  &dbus_glib_g_uptime_object_info);
}

static void g_uptime_init(GUptime *self)
{
  DBusGProxy *proxy = NULL;
  GUptimeClass *klass = G_UPTIME_GET_CLASS(self);
  GError *error = NULL;
  DBusConnection *connection = dbus_g_connection_get_connection(klass->dbus);
  DBusError dbus_error;

  dbus_g_connection_register_g_object(klass->dbus,
				      "/br/eti/kov/GUptime",
				      G_OBJECT(self));

  proxy = dbus_g_proxy_new_for_name(klass->dbus,
				    "br.eti.kov.GUptime",
				    "/br/eti/kov/GUptime",
				    "br.eti.kov.GUptime");

  dbus_error_init(&dbus_error);
  if(dbus_bus_request_name(connection,
			   "br.eti.kov.GUptime",
			   0, &dbus_error) == -1)
    {
      g_error("Failed to get well-known name:\n %s: %s\n",
	      dbus_error.name, dbus_error.message);
      exit(1);
    }

  dbus_bus_add_match (connection,
		      "type='signal'"
		      ",interface='"DBUS_INTERFACE_DBUS"'"
		      ",sender='"DBUS_SERVICE_DBUS"'"
		      ",member='NameOwnerChanged'",
		      &dbus_error);

  dbus_bus_add_match (connection,
		      "type='signal',sender='org.freedesktop.ConsoleKit'",
		      &dbus_error);

  dbus_connection_add_filter(connection, g_uptime_handle_dbus_message,
			     (void*)self, NULL);

  g_object_unref(proxy);
}

static PolKitCaller* g_uptime_get_caller_from_message(GUptime *self,
                                                      DBusMessage *message)
{
  GUptimeClass *klass = G_UPTIME_GET_CLASS(self);
  PolKitTracker *pk_tracker = klass->pk_tracker;
  PolKitCaller *pk_caller = NULL;
  DBusError dbus_error;
  const gchar *sender = NULL;

  dbus_error_init(&dbus_error);

  sender = dbus_message_get_sender(message);
  pk_caller = polkit_tracker_get_caller_from_dbus_name(pk_tracker,
						       sender,
						       &dbus_error);
  if(pk_caller == NULL)
    {
      if(dbus_error_is_set(&dbus_error))
	{
	  g_error("Failed to get caller from dbus: %s: %s\n",
		  dbus_error.name, dbus_error.message);
	  return NULL;
	}
    }

  return pk_caller;
}

DBusHandlerResult g_uptime_handle_dbus_message(DBusConnection *conn,
                                               DBusMessage *message,
                                               void *user_data)
{
  GUptime *self = G_UPTIME(user_data);
  GUptimeClass *klass = G_UPTIME_GET_CLASS(self);
  DBusGConnection *dbus = klass->dbus;
  DBusConnection *connection = dbus_g_connection_get_connection(dbus);
  PolKitContext *pk_context = klass->pk_context;
  PolKitTracker *pk_tracker = klass->pk_tracker;
  PolKitCaller *pk_caller = NULL;
  PolKitAction *pk_action = NULL;
  PolKitResult pk_result;
  PolKitError *pk_error = NULL;

  /*  pass NameOwnerChanged signals from the bus and ConsoleKit to PolKitTracker */
  if (dbus_message_is_signal (message, DBUS_INTERFACE_DBUS, "NameOwnerChanged") ||
      g_str_has_prefix (dbus_message_get_interface (message),
			"org.freedesktop.ConsoleKit"))
    polkit_tracker_dbus_func (pk_tracker, message);

  if(dbus_message_is_method_call(message, "br.eti.kov.GUptime", "get"))
    {
      g_print("Method called!\n");

      pk_caller = g_uptime_get_caller_from_message(self, message);
      if(pk_caller != NULL)
        {
	  pid_t caller_pid;
	  uid_t caller_uid;

	  polkit_caller_get_pid(pk_caller, &caller_pid);
	  polkit_caller_get_uid(pk_caller, &caller_uid);

	  g_print("Caller from pid %d, uid %d\n", caller_pid, caller_uid);
        }

      pk_action = polkit_action_new();
      polkit_action_set_action_id(pk_action, "br.eti.kov.guptime.get");

      pk_result = polkit_context_is_caller_authorized(pk_context,
						      pk_action,
						      pk_caller,
						      TRUE,
						      &pk_error);
      if(pk_result != POLKIT_RESULT_YES)
	{
	  DBusMessage *reply;
	  const gchar *pk_result_str = 
	    polkit_result_to_string_representation(pk_result);

	  reply = dbus_message_new_error(message,
					 DBUS_ERROR_FAILED,
					 pk_result_str);
	  dbus_connection_send(connection, reply, NULL);
	  dbus_message_unref(reply);

	  return DBUS_HANDLER_RESULT_HANDLED;
	}
    }

  return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

gboolean g_uptime_get(GUptime *self, gdouble *uptime, GError **error)
{
  glibtop_uptime buf;
    
  g_print("Method really called!\n");
  glibtop_get_uptime(&buf);
  *uptime = buf.uptime;

  return TRUE;
}

