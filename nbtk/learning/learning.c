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
#include <nbtk/nbtk.h>

int main (int argc, char **argv)
{
    ClutterActor *stage;
    NbtkWidget *box;
    NbtkWidget *entry;

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

    clutter_actor_show_all(stage);

    clutter_main();

    return 0;
}
