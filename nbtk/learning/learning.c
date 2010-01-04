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
#include <nbtk/nbtk.h>

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

int main (int argc, char **argv)
{
    ClutterActor *stage;
    ClutterModel *model;
    NbtkWidget *box;
    NbtkWidget *entry;
    NbtkWidget *list_view;
    NbtkWidget *scroll_view;

    clutter_init(&argc, &argv);

    stage = clutter_stage_get_default();

    box = nbtk_box_layout_new();
    nbtk_box_layout_set_vertical(NBTK_BOX_LAYOUT(box), TRUE);
    clutter_actor_set_size(CLUTTER_ACTOR(box), CLUTTER_STAGE_WIDTH(), CLUTTER_STAGE_HEIGHT());

    clutter_container_add(CLUTTER_CONTAINER(stage), CLUTTER_ACTOR(box), NULL);

    entry = nbtk_entry_new(NULL);
    nbtk_entry_set_hint_text(NBTK_ENTRY(entry), "Type a name");
    clutter_container_add(CLUTTER_CONTAINER(box), CLUTTER_ACTOR(entry), NULL);
    clutter_container_child_set(CLUTTER_CONTAINER(box), CLUTTER_ACTOR(entry),
                                "x-fill", TRUE, NULL);

    scroll_view = nbtk_scroll_view_new();
    clutter_container_add(CLUTTER_CONTAINER(box), CLUTTER_ACTOR(scroll_view), NULL);
    clutter_container_child_set(CLUTTER_CONTAINER(box), CLUTTER_ACTOR(scroll_view),
                                "x-fill", TRUE,
                                "y-fill", TRUE,
                                NULL);

    model = create_model();

    list_view = nbtk_list_view_new();
    nbtk_list_view_set_item_type(NBTK_LIST_VIEW(list_view), NBTK_TYPE_LABEL);
    nbtk_box_layout_set_vertical(NBTK_BOX_LAYOUT(list_view), TRUE);
    nbtk_list_view_set_model(NBTK_LIST_VIEW(list_view), model);
    nbtk_list_view_add_attribute(NBTK_LIST_VIEW(list_view), "text", COLUMN_NAME);
    clutter_container_add(CLUTTER_CONTAINER(scroll_view), CLUTTER_ACTOR(list_view), NULL);

    clutter_actor_show_all(stage);

    clutter_main();

    return 0;
}
