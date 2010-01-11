/*
 * Copyright © 2010 Collabora Ltd.
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

int main (int argc, char **argv)
{
    ClutterActor *stage;
    ClutterActor *table;
    ClutterActor *notebook;
    ClutterActor *rectangle;
    ClutterColor color;

    clutter_init (&argc, &argv);

    stage = clutter_stage_get_default ();
    clutter_actor_set_size (stage, 800, 600);

    clutter_stage_set_user_resizable (CLUTTER_STAGE (stage), TRUE);

    table = mx_table_new ();
    clutter_actor_set_size (table, 800, 600);
    clutter_container_add_actor (CLUTTER_CONTAINER (stage), table);

    /* If you comment the addition of this rectangle, the notebookk
     * displays the red rectangle correctly. If this is added, the red
     * rectangle does not show up. */
    clutter_color_from_string (&color, "yellow");
    rectangle = clutter_rectangle_new_with_color (&color);
    mx_table_add_actor_with_properties (MX_TABLE (table),
                                        rectangle,
                                        0, 0,
                                        "y-expand", TRUE,
                                        "x-expand", TRUE,
                                        "x-fill", TRUE,
                                        "y-fill", TRUE,
                                        NULL);

    notebook = mx_notebook_new ();
    mx_table_add_actor_with_properties (MX_TABLE (table),
                                        notebook,
                                        0, 1,
                                        "y-expand", TRUE,
                                        "x-expand", TRUE,
                                        "x-fill", TRUE,
                                        "y-fill", TRUE,
                                        NULL);

    clutter_color_from_string (&color, "red");
    rectangle = clutter_rectangle_new_with_color (&color);
    clutter_container_add_actor (CLUTTER_CONTAINER (notebook), rectangle);

    clutter_actor_show_all (stage);

    clutter_main ();

    return 0;
}
