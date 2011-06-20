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

#include <libjana-ecal/jana-ecal.h>

static void
query_progress_cb(JanaStoreView *view, gint percent, gpointer data)
{
    GList *matches = jana_store_view_get_matches(view);
    g_debug("matches: %d (%d%%)", g_list_length(matches), percent);

    for(; matches; matches = matches->next) {
        JanaStoreViewMatch *match = (JanaStoreViewMatch*)matches->data;
        g_debug("Match: %s", match->data);
    }
}

static void
calendar_store_opened_cb(JanaStore *calendar_store, gpointer data)
{
    JanaStoreView *view = jana_store_get_view(calendar_store);

    g_signal_connect(view, "progress",
                     G_CALLBACK(query_progress_cb), NULL);

    jana_store_view_add_match(view, JANA_STORE_VIEW_ANYFIELD, "test");

    jana_store_view_start(view);
}

int main(int argc, char **argv)
{
    GMainLoop *loop;
    JanaStore *calendar_store;

    g_type_init();

    calendar_store = jana_ecal_store_new(JANA_COMPONENT_EVENT);
    g_signal_connect(calendar_store, "opened",
                     G_CALLBACK(calendar_store_opened_cb), NULL);

    jana_store_open(calendar_store);

    loop = g_main_loop_new(NULL, TRUE);
    g_main_loop_run(loop);

    return 0;
}
