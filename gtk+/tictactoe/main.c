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
#include "gtktictactoe.h"

void
victory_reached_cb(GtkWidget *tictactoe, GtkTicTacToeMark mark)
{
  GtkWidget *dialog;
  GtkWindow *parent;

  parent = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(tictactoe)));

  dialog = gtk_message_dialog_new_with_markup(parent,
                                              GTK_DIALOG_MODAL,
                                              GTK_MESSAGE_INFO,
                                              GTK_BUTTONS_CLOSE,
                                              "<b><big>You win!</big></b>");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

int  main(int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *tictactoe;

  gtk_init();
  
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(G_OBJECT(window), "close-request",
                   G_CALLBACK(gtk_main_quit), NULL);
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

  tictactoe = gtk_tictactoe_new();
  g_signal_connect(G_OBJECT(tictactoe), "victory-reached",
                   G_CALLBACK(victory_reached_cb), NULL);

/*   gtk_tictactoe_mark(GTK_TICTACTOE(tictactoe), 0, 0, GTK_TICTACTOE_MARK_X); */
/*   gtk_tictactoe_mark(GTK_TICTACTOE(tictactoe), 0, 1, GTK_TICTACTOE_MARK_O); */
/*   gtk_tictactoe_mark(GTK_TICTACTOE(tictactoe), 0, 2, GTK_TICTACTOE_MARK_O); */
/*   gtk_tictactoe_mark(GTK_TICTACTOE(tictactoe), 1, 0, GTK_TICTACTOE_MARK_O); */
/*   gtk_tictactoe_mark(GTK_TICTACTOE(tictactoe), 1, 1, GTK_TICTACTOE_MARK_X); */
/*   gtk_tictactoe_mark(GTK_TICTACTOE(tictactoe), 1, 2, GTK_TICTACTOE_MARK_O); */
/*   gtk_tictactoe_mark(GTK_TICTACTOE(tictactoe), 2, 0, GTK_TICTACTOE_MARK_X); */
/*   gtk_tictactoe_mark(GTK_TICTACTOE(tictactoe), 2, 1, GTK_TICTACTOE_MARK_O); */
/*   gtk_tictactoe_mark(GTK_TICTACTOE(tictactoe), 2, 2, GTK_TICTACTOE_MARK_X); */

  gtk_container_add(GTK_CONTAINER(window), tictactoe);
  gtk_widget_show(window);

  gtk_main();

  return 0;
}
