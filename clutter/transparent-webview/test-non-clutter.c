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

#include <webkit/webkit.h>

const char* html_code = "<video width='320' height='200' src='file:////home/kov/shell-20100202-2.ogv'></video>";

int main (int argc, char **argv)
{
    GtkWidget *window;
    GtkWidget *embed;
    GtkWidget *widget;
    GError *error = NULL;

    g_thread_init(NULL);
    gtk_init(&argc, &argv);

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

    /* Web pages */
    widget = webkit_web_view_new();
    webkit_web_view_set_transparent(WEBKIT_WEB_VIEW(widget), TRUE);
    webkit_web_view_load_string(WEBKIT_WEB_VIEW(widget), html_code, "text/html", "UTF-8", "file://");

    gtk_container_add(GTK_CONTAINER(window), widget);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
