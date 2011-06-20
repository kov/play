/*
 * Copyright © 2010 Gustavo Noronha Silva.
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

#include <clutter/clutter.h>
#include <glib.h>
#include <gio/gio.h>
#include <mx/mx.h>

gboolean remove_actor (ClutterActor *rectangle)
{
    ClutterContainer *notebook = CLUTTER_CONTAINER (g_object_get_data (G_OBJECT (rectangle), "notebook"));
    clutter_container_remove_actor (notebook, rectangle);
    return FALSE;
}

int main (int argc, char **argv)
{
    ClutterActor *stage;
    ClutterActor *notebook;
    ClutterActor *rectangle;
    ClutterColor color;

    clutter_init (&argc, &argv);

    stage = clutter_stage_get_default ();
    clutter_actor_set_size (stage, 800, 600);

    notebook = mx_notebook_new ();
    clutter_actor_set_size (notebook, 800, 600);
    clutter_container_add_actor (CLUTTER_CONTAINER (stage), notebook);

    clutter_color_from_string (&color, "red");
    rectangle = clutter_rectangle_new_with_color (&color);
    clutter_actor_set_size (rectangle, 800, 600);
    clutter_container_add_actor (CLUTTER_CONTAINER (notebook), rectangle);

    g_object_set_data (G_OBJECT (rectangle), "notebook", notebook);
    g_timeout_add (1000, (GSourceFunc)remove_actor, rectangle);

    clutter_color_from_string (&color, "yellow");
    rectangle = clutter_rectangle_new_with_color (&color);
    clutter_actor_set_size (rectangle, 800, 600);
    clutter_container_add_actor (CLUTTER_CONTAINER (notebook), rectangle);

    clutter_actor_show_all (stage);

    clutter_main ();

    return 0;
}
