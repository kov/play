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

#ifndef __GUPTIME_H__
#define __GUPTIME_H__ 1

#include <glib-object.h>
#include <dbus/dbus-glib.h>
#include <polkit-dbus/polkit-dbus.h>

typedef struct {
    GObject parent;
} GUptime;

typedef struct {
    GObjectClass parent;

    DBusGConnection *dbus;
    PolKitContext *pk_context;
    PolKitTracker *pk_tracker;
} GUptimeClass;

GType g_uptime_get_type(void);

#define G_TYPE_UPTIME (g_uptime_get_type())
#define G_UPTIME(object) (G_TYPE_CHECK_INSTANCE_CAST ((object), G_TYPE_UPTIME, GUptime))
#define G_UPTIME_GET_CLASS(object)  (G_TYPE_INSTANCE_GET_CLASS ((object), G_TYPE_UPTIME, GUptimeClass))

gboolean g_uptime_get(GUptime *self, gdouble *uptime, GError **error);

#endif
