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

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <webkit/webkitwebview.h>

void location_activate_cb(GtkWidget *location_entry, gpointer user_data)
{
  GtkWidget *webview = GTK_WIDGET(user_data);
  const gchar *uri = gtk_entry_get_text(GTK_ENTRY(location_entry));

  webkit_web_view_open(WEBKIT_WEB_VIEW(webview), uri);
}

int main(int argc, char **argv)
{
  GladeXML *gui;
  GtkWidget *location_entry;
  GtkWidget *notebook;
  GtkWidget *webview;

  gtk_init(&argc, &argv);

  gui = glade_xml_new("data/main.glade", NULL, NULL);
  glade_xml_signal_autoconnect(gui);

  notebook = glade_xml_get_widget(gui, "main_notebook");
  
  webview = webkit_web_view_new();
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), webview, NULL);
  gtk_widget_show(webview);

  location_entry = glade_xml_get_widget(gui, "location_entry");
  g_signal_connect(G_OBJECT(location_entry), "activate",
                   G_CALLBACK(location_activate_cb), webview);

  gtk_main();

  return 0;
}
