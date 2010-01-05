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

enum {
    COLUMN_NAME,
    COLUMN_SIZE,

    N_COLUMNS
};

static ClutterModel*
create_model()
{
    ClutterModel *model;
    GFile *directory;
    GFileEnumerator *enumerator;
    GFileInfo *info;
    GError *error = NULL;

    model = clutter_list_model_new(N_COLUMNS,
                                   G_TYPE_STRING, "File name",
                                   G_TYPE_UINT64, "Size (bytes)");

    directory = g_file_new_for_path(g_get_home_dir());
    enumerator = g_file_enumerate_children(directory,
                                           "standard::*",
                                           G_FILE_QUERY_INFO_NONE,
                                           NULL,
                                           &error);
    if (error)
        g_error("Failed to query directory contents.");

    while (info = g_file_enumerator_next_file(enumerator, NULL, &error)) {
        char *name;
        guint64 size;

        if (error)
            g_error("Failed to query a file.");

        name = g_file_info_get_attribute_as_string(info,
                                                   G_FILE_ATTRIBUTE_STANDARD_NAME);
        size = g_file_info_get_attribute_uint64(info,
                                                G_FILE_ATTRIBUTE_STANDARD_SIZE);

        clutter_model_append(model, COLUMN_NAME, name, COLUMN_SIZE, size, -1);
    }

    return model;
}

static void
scroll_view_allocation_notify_cb (ClutterActor *stage,
                                  GParamSpec   *pspec,
                                  ClutterActor *scroll_view)
{
    gfloat width, height;

    clutter_actor_get_size(stage, &width, &height);
    clutter_actor_set_size(scroll_view, width - 100, height - 120);
}

static void
entry_allocation_notify_cb (ClutterActor *stage,
                            GParamSpec   *pspec,
                            ClutterActor *entry)
{
    gfloat width, height;

    clutter_actor_get_size(stage, &width, &height);
    clutter_actor_set_width(entry, width - 100);
}

int main (int argc, char **argv)
{
    ClutterActor *stage;
    ClutterModel *model;
    ClutterActor *entry;
    ClutterActor *list_view;
    ClutterActor *scroll_view;

    clutter_init(&argc, &argv);

    stage = clutter_stage_get_default();

    entry = mx_entry_new(NULL);
    mx_entry_set_hint_text(MX_ENTRY(entry), "Type a name");
    clutter_container_add_actor(CLUTTER_CONTAINER(stage), entry);
    clutter_actor_set_position(entry, 50.0, 20.0);

    scroll_view = mx_scroll_view_new();
    clutter_container_add_actor(CLUTTER_CONTAINER(stage), scroll_view);
    clutter_actor_set_position(scroll_view, 50.0, 100.0);

    model = create_model();

    list_view = mx_list_view_new();
    mx_list_view_set_item_type(MX_LIST_VIEW(list_view), MX_TYPE_LABEL);
    mx_box_layout_set_vertical(MX_BOX_LAYOUT(list_view), TRUE);
    mx_list_view_set_model(MX_LIST_VIEW(list_view), model);
    mx_list_view_add_attribute(MX_LIST_VIEW(list_view), "text", COLUMN_NAME);
    clutter_container_add(CLUTTER_CONTAINER(scroll_view), CLUTTER_ACTOR(list_view), NULL);

    g_signal_connect(stage, "notify::allocation",
                     G_CALLBACK(scroll_view_allocation_notify_cb), scroll_view);
    g_signal_connect(stage, "notify::allocation",
                     G_CALLBACK(entry_allocation_notify_cb), entry);

    clutter_actor_show_all(stage);

    clutter_main();

    return 0;
}
