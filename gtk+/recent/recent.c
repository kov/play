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

void add_recent_cb(GtkWidget *widget, gpointer data)
{
    GtkRecentManager *manager = gtk_recent_manager_get_default();
    gtk_recent_manager_add_item(manager, "file:///etc/fstab");
}

static const GtkActionEntry menu_entries[] =
{
    /* File menu.  */
    { "File", NULL, "_File" },
    { "FileNew", GTK_STOCK_NEW, NULL, NULL,
      "New", NULL },
};

int main(int argc, char **argv)
{
    GError *error = NULL;

    GladeXML *gui;
    GtkWidget *button;
    GtkWidget *vbox;
    GtkWidget *recent_files;

    GtkUIManager *ui_manager;
    GtkActionGroup *action_group;
    GtkWidget *recent_files_menu;

    gtk_init(&argc, &argv);

    gui = glade_xml_new("recent.glade", NULL, NULL);
    glade_xml_signal_autoconnect(gui);
    vbox = glade_xml_get_widget(gui, "main_vbox");
 
    action_group = gtk_action_group_new("RecentActions");
    gtk_action_group_add_actions(action_group, menu_entries,
                                 G_N_ELEMENTS(menu_entries), NULL);

    ui_manager = gtk_ui_manager_new();
    gtk_ui_manager_insert_action_group(ui_manager, action_group, 0);
    gtk_ui_manager_add_ui_from_file(ui_manager, "ui.xml", &error);
    if(error)
    {
        g_error("Failed to load XML: %s", error->message);
        return 1;
    }
    recent_files_menu = gtk_ui_manager_get_widget(ui_manager, "/MenuBar");
    gtk_box_pack_start_defaults(GTK_BOX(vbox), recent_files_menu);

    {
        GtkWidget *submenu;

        submenu = gtk_menu_item_new();
        gtk_widget_show(submenu);

        recent_files_menu = gtk_ui_manager_get_widget(ui_manager, "/MenuBar/FileRecentMenu");
        recent_files = gtk_recent_chooser_menu_new();
        gtk_widget_show(recent_files);
        
        gtk_menu_shell_append(GTK_MENU_SHELL(recent_files_menu), submenu);
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(submenu), recent_files);
    }


    recent_files = gtk_recent_chooser_widget_new();
    gtk_box_pack_start_defaults(GTK_BOX(vbox), recent_files);
    gtk_widget_show(recent_files);

    gtk_main();

    return 0;
}
