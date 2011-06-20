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

const char* html_code = "<video controls width='320' height='200' src='file:////home/kov/shell-20100202-2.ogv'></video>";

static void
allocation_notify_cb (GObject *object, GParamSpec *pspec, ClutterActor *actor)
{
    ClutterActor *stage = CLUTTER_ACTOR(object);
    gfloat width;
    gfloat height;

    clutter_actor_get_size(stage, &width, &height);
    clutter_actor_set_size(actor, width, height);
}

int main (int argc, char **argv)
{
    GtkWidget *window;
    GtkWidget *embed;
    ClutterActor *stage;
    ClutterActor *webpage;
    GtkWidget *widget;
    ClutterColor color;
    ClutterPath *path;
    ClutterTimeline *timeline;
    ClutterAlpha *alpha;
    GError *error = NULL;

    g_thread_init(NULL);
    gtk_clutter_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    GdkScreen *screen = NULL;
    GdkColormap *colormap = NULL;

    screen = gdk_screen_get_default();

    if (screen)
        colormap = gdk_screen_get_rgba_colormap(screen);

    if (colormap)
        gtk_widget_set_colormap(window, colormap);
    else
        g_debug("NONONO...");

    gtk_window_set_default_size(GTK_WINDOW(window), 640, 768);
    g_signal_connect(window, "delete-event",
                     G_CALLBACK(gtk_main_quit), NULL);

    embed = gtk_clutter_embed_new();
    if (colormap)
        gtk_widget_set_colormap(embed, colormap);
    gtk_container_add(GTK_CONTAINER(window), embed);

    clutter_color_from_string(&color, "yellow");
    stage = gtk_clutter_embed_get_stage(GTK_CLUTTER_EMBED(embed));
    clutter_stage_set_color(CLUTTER_STAGE(stage), &color);

    /* Web pages */
    widget = webkit_web_view_new();
    /* if (colormap) */
    /*     gtk_widget_set_colormap(widget, colormap); */

    webkit_web_view_set_transparent(WEBKIT_WEB_VIEW(widget), TRUE);

    webpage = gtk_clutter_actor_new_with_contents(widget);
    clutter_container_add_actor(CLUTTER_CONTAINER(stage), webpage);

    clutter_actor_set_opacity(webpage, 150);

    g_signal_connect (stage, "notify::allocation",
                      G_CALLBACK (allocation_notify_cb),
                      webpage);

    webkit_web_view_load_string(WEBKIT_WEB_VIEW(widget), html_code, "text/html", "UTF-8", "file://");

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
