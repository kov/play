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
#include <clutter-gtk/clutter-gtk.h>
#include <webkit/webkit.h>

typedef struct {
    WebKitWebView *view;
    ClutterActor *actor;
    gboolean ready;
} Slide;

Slide *current;
Slide *next;

gboolean started = FALSE;
int current_index = 0;
const char* uris[] = {
    "http://debian.org/",
    "http://gnome.org/",
    "http://webkit.org/",
    "http://kov.eti.br/",
    "http://planet.gnome.org/",
    "http://planet.debian.org/",
    "http://google.com/",
    "http://bing.com/",
    NULL
};

static void
allocation_notify_cb (GObject *object, GParamSpec *pspec, ClutterActor *actor)
{
    ClutterActor *stage = CLUTTER_ACTOR(object);
    gfloat width;
    gfloat height;

    clutter_actor_get_size(stage, &width, &height);
    clutter_actor_set_size(actor, width, height);
}

void load_status_cb(GObject *object, GParamSpec *pspec, Slide *slide)
{
    WebKitLoadStatus status = webkit_web_view_get_load_status(WEBKIT_WEB_VIEW(object));

    if (status == WEBKIT_LOAD_FINISHED
        || status == WEBKIT_LOAD_FAILED)
        slide->ready = TRUE;
}

gboolean next_page_cb(gpointer data)
{
    Slide *tmp;

    if (!next->ready)
        return TRUE;

    /* Stop monitoring */
    g_signal_handlers_disconnect_by_func(next->view,
                                         G_CALLBACK(load_status_cb),
                                         next);

    /* Apply transitions */
    clutter_actor_set_position(next->actor, 640, 0);
    clutter_actor_animate(next->actor, CLUTTER_EASE_IN_OUT_QUAD, 1000,
                          "x", 0.0,
                          NULL);

    clutter_actor_animate(current->actor, CLUTTER_EASE_IN_OUT_QUAD, 1000,
                          "x", -640.0,
                          NULL);

    tmp = g_slice_new(Slide);

    tmp->view = current->view;
    tmp->actor = current->actor;

    current->view = next->view;
    current->actor = next->actor;

    next->view = tmp->view;
    next->actor = tmp->actor;
    next->ready = FALSE;

    current_index++;
    if (uris[current_index] == NULL)
        current_index = 0;

    /* We only want to switch if we are done loading */
    webkit_web_view_load_uri(next->view, uris[current_index]);
    g_signal_connect(next->view, "notify::load-status",
                     G_CALLBACK(load_status_cb), next);

    g_slice_free(Slide, tmp);

    return TRUE;
}

int main (int argc, char **argv)
{
    GtkWidget *window;
    GtkWidget *embed;
    ClutterActor *stage;
    ClutterActor *webpage;
    GtkWidget *scrolled_window;
    GtkWidget *widget;
    ClutterColor color;
    ClutterPath *path;
    ClutterTimeline *timeline;
    ClutterAlpha *alpha;
    GError *error = NULL;

    g_thread_init(NULL);
    gtk_clutter_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 768);
    g_signal_connect(window, "delete-event",
                     G_CALLBACK(gtk_main_quit), NULL);

    embed = gtk_clutter_embed_new();
    gtk_container_add(GTK_CONTAINER(window), embed);

    clutter_color_from_string(&color, "black");
    stage = gtk_clutter_embed_get_stage(GTK_CLUTTER_EMBED(embed));
    clutter_stage_set_color(CLUTTER_STAGE(stage), &color);

    current = g_slice_new(Slide);
    next = g_slice_new(Slide);

    /* Initialize both to not ready */
    current->ready = next->ready = FALSE;

    /* Web pages */
    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    widget = webkit_web_view_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), widget);
    gtk_widget_show_all(scrolled_window);

    webpage = gtk_clutter_actor_new_with_contents(scrolled_window);
    clutter_container_add_actor(CLUTTER_CONTAINER(stage), webpage);

    g_signal_connect (stage, "notify::allocation",
                      G_CALLBACK (allocation_notify_cb),
                      webpage);

    /* Current page */
    current->view = WEBKIT_WEB_VIEW(widget);
    current->actor = webpage;

    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    widget = webkit_web_view_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), widget);
    gtk_widget_show_all(scrolled_window);

    webpage = gtk_clutter_actor_new_with_contents(scrolled_window);
    clutter_actor_set_position(webpage, 640, 0);
    clutter_container_add_actor(CLUTTER_CONTAINER(stage), webpage);

    g_signal_connect (stage, "notify::allocation",
                      G_CALLBACK (allocation_notify_cb),
                      webpage);

    /* Next page */
    next->view = WEBKIT_WEB_VIEW(widget);
    next->actor = webpage;
    next->ready = FALSE;

    webkit_web_view_load_uri(next->view, uris[0]);
    g_signal_connect(next->view, "notify::load-status",
                     G_CALLBACK(load_status_cb), next);

    g_timeout_add_seconds(3, next_page_cb, NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
