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
#include <glib-object.h>

#include "gtktictactoe.h"

G_DEFINE_TYPE(GtkTicTacToe, gtk_tictactoe, GTK_TYPE_DRAWING_AREA);

static gboolean
on_expose (GtkWidget* widget, GdkEventExpose* pEvent, gpointer data)
{
  cairo_t  *cairo_context = gdk_cairo_create(widget->window);

  g_message("Allocation: %f x %f",
            (gdouble)widget->allocation.width,
            (gdouble)widget->allocation.height);

  cairo_set_source_rgb(cairo_context, 0.0, 0.0, 0.0);
  cairo_rectangle (cairo_context, 0.0, 0.0, 200.0, 50.0);
  cairo_fill(cairo_context);
  cairo_destroy(cairo_context);

  return FALSE;
}

static void
gtk_tictactoe_finalize (GObject *object)
{
  G_OBJECT_CLASS (gtk_tictactoe_parent_class)->finalize (object);
}

static void
gtk_tictactoe_class_init (GtkTicTacToeClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->finalize = gtk_tictactoe_finalize;
  g_warning("class init");
}

static void
gtk_tictactoe_init (GtkTicTacToe *tictactoe)
{
  g_signal_connect (G_OBJECT (tictactoe), "expose-event",
                    G_CALLBACK (on_expose), NULL);
}

GtkWidget * 
gtk_tictactoe_new ()
{
  GtkTicTacToe *tictactoe;
  tictactoe = g_object_new (GTK_TYPE_TICTACTOE, NULL);  

  return GTK_WIDGET (tictactoe);
}
