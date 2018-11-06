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
#include <math.h>

#include "gtktictactoe-marshal.h"
#include "gtktictactoe.h"

enum
  {
    MARK_ADDED,
    VICTORY_REACHED,
    LAST_SIGNAL
  };

static guint gtk_tictactoe_signals[LAST_SIGNAL] = { 0, };

GType
gtk_tictactoe_mark_get_type()
{
  static GType type = 0;
  static const GEnumValue values[] =
    {
      { GTK_TICTACTOE_MARK_X, "GTK_TICTACTOE_MARK_X", "mark-x" },
      { GTK_TICTACTOE_MARK_O, "GTK_TICTACTOE_MARK_O", "mark-o" },
      { 0, NULL, NULL }
    };

  if(type == 0)
    type = g_enum_register_static ("GtkTicTacToeMark", values);

  return type;
}

G_DEFINE_TYPE(GtkTicTacToe, gtk_tictactoe, GTK_TYPE_DRAWING_AREA);

static void
on_draw (GtkDrawingArea* drawing_area, cairo_t* cairo_context, int width, int height, gpointer data)
{
  GtkTicTacToe *tictactoe = GTK_TICTACTOE(drawing_area);
  int i, j;

  g_message("Allocation: %f x %f",
            (gdouble)width,
            (gdouble)height);

  cairo_scale(cairo_context,
              (gdouble)width,
              (gdouble)height);

  cairo_set_source_rgb(cairo_context, 1.0, 1.0, 1.0);
  cairo_rectangle (cairo_context, 0.0, 0.0, 1.0, 1.0);
  cairo_fill(cairo_context);

  cairo_set_line_width(cairo_context, 0.015);

  /* draws the vertical lines */
  cairo_set_source_rgb(cairo_context, 0.0, 0.0, 0.0);
  cairo_move_to(cairo_context, 0.35, 0.05);
  cairo_line_to(cairo_context, 0.35, 0.95);
  cairo_stroke(cairo_context);

  cairo_move_to(cairo_context, 0.65, 0.05);
  cairo_line_to(cairo_context, 0.65, 0.95);
  cairo_stroke(cairo_context);

  /* draws the horizontal lines */
  cairo_move_to(cairo_context, 0.05, 0.35);
  cairo_line_to(cairo_context, 0.95, 0.35);
  cairo_stroke(cairo_context);

  cairo_move_to(cairo_context, 0.05, 0.65);
  cairo_line_to(cairo_context, 0.95, 0.65);
  cairo_stroke(cairo_context);

  /* draws the marks */
  for(i = 0; i < 3; i++)
    {
      for(j = 0; j < 3; j++)
        {
          gdouble x_offset = 0.10;
          gdouble y_offset = 0.10;
          
          x_offset += (0.30 * i);

          y_offset += (0.30 * j);

          if(tictactoe->data[i][j] == GTK_TICTACTOE_MARK_X)
            {
              /* drawing the X */
              cairo_set_source_rgb(cairo_context, 1.0, 0.0, 0.0);
              cairo_move_to(cairo_context, x_offset, y_offset);
              cairo_line_to(cairo_context, x_offset + 0.20, y_offset + 0.20);
              cairo_stroke(cairo_context);

              cairo_move_to(cairo_context, x_offset + 0.20, y_offset);
              cairo_line_to(cairo_context, x_offset, y_offset + 0.20);
              cairo_stroke(cairo_context);
            }
          else if(tictactoe->data[i][j] == GTK_TICTACTOE_MARK_O)
            {
              cairo_set_source_rgb(cairo_context, 0.0, 0.0, 1.0);
              cairo_arc(cairo_context, x_offset + 0.10, y_offset + 0.10, 0.10, 0, 2 * M_PI);
              cairo_stroke(cairo_context);
            }
        }
    }
}

static void
mark_added_cb(GtkTicTacToe *tictactoe, guint x, guint y, GtkTicTacToeMark mark)
{
  gboolean victory = FALSE;

  g_message("Mark added at %dx%d: %d", x, y, (gint)mark);
  
  /* check if we have a victory vertically */
  if(((tictactoe->data[x][0] != 0) && (tictactoe->data[x][1] != 0) && (tictactoe->data[x][1] != 0)) &&
     ((tictactoe->data[x][0] == tictactoe->data[x][1]) && (tictactoe->data[x][1] == tictactoe->data[x][2])))
    {
      victory = TRUE;
      goto check_victory;
    }

  /* check if we have a victory horizontally */
  if(((tictactoe->data[0][y] != 0) && (tictactoe->data[1][y] != 0) && (tictactoe->data[2][y] != 0)) &&
     ((tictactoe->data[0][y] == tictactoe->data[1][y]) && (tictactoe->data[1][y] == tictactoe->data[2][y])))
    {
      victory = TRUE;
      goto check_victory;
    }

  /* or on the diagonals */
  if( ((tictactoe->data[0][0] != 0) && (tictactoe->data[1][1] != 0) && (tictactoe->data[2][2] != 0)) &&
      ((tictactoe->data[0][0] == tictactoe->data[1][1]) && (tictactoe->data[1][1] == tictactoe->data[2][2])) )
    {
      victory = TRUE;
      goto check_victory;
    }

  if( ((tictactoe->data[2][0] != 0) && (tictactoe->data[1][1] != 0) && (tictactoe->data[0][2] != 0)) &&
      ((tictactoe->data[2][0] == tictactoe->data[1][1]) && (tictactoe->data[1][1] == tictactoe->data[0][2])) )
    {
      victory = TRUE;
      goto check_victory;
    }

 check_victory:
  if(victory)
    g_signal_emit(tictactoe, gtk_tictactoe_signals[VICTORY_REACHED], 0, mark);
}

void
on_button_pressed_cb(GtkGesture *gesture, int n_press, double x, double y, GtkWidget *widget)
{
  GtkTicTacToe *tictactoe = GTK_TICTACTOE(widget);
  gdouble width, height;
  GtkAllocation allocation;
  gdouble relative_x, relative_y;
  guint coord_x, coord_y;

  gtk_widget_get_allocation(widget, &allocation);

  width = (gdouble)allocation.width;
  height = (gdouble)allocation.height;

  relative_x = x / width;
  relative_y = y / height;

  if(relative_x <= 0.35)
    coord_x = 0;
  else if((relative_x > 0.35) && (relative_x <= 0.65))
    coord_x = 1;
  else
    coord_x = 2;

  if(relative_y <= 0.35)
    coord_y = 0;
  else if((relative_y > 0.35) && (relative_y <= 0.65))
    coord_y = 1;
  else
    coord_y = 2;

  g_message("Click at %dx%d", coord_x, coord_y);
  gtk_tictactoe_mark(tictactoe, coord_x, coord_y, tictactoe->current_mark);
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

  gtk_tictactoe_signals[MARK_ADDED] = g_signal_new("mark-added",
                                                   GTK_TYPE_TICTACTOE,
                                                   G_SIGNAL_RUN_LAST|G_SIGNAL_ACTION,
                                                   0,
                                                   NULL,
                                                   NULL,
                                                   gtk_tictactoe_marshal_VOID__INT_INT_ENUM,
                                                   G_TYPE_NONE,
                                                   3,
                                                   G_TYPE_INT,
                                                   G_TYPE_INT,
                                                   GTK_TYPE_TICTACTOE_MARK);

  gtk_tictactoe_signals[VICTORY_REACHED] = g_signal_new("victory-reached",
                                                        GTK_TYPE_TICTACTOE,
                                                        G_SIGNAL_RUN_LAST|G_SIGNAL_ACTION,
                                                        0,
                                                        NULL,
                                                        NULL,
                                                        g_cclosure_marshal_VOID__ENUM,
                                                        G_TYPE_NONE,
                                                        1,
                                                        GTK_TYPE_TICTACTOE_MARK);

  object_class->finalize = gtk_tictactoe_finalize;
}

static void
gtk_tictactoe_init (GtkTicTacToe *tictactoe)
{
  GtkGesture *gesture;

  tictactoe->current_mark = GTK_TICTACTOE_MARK_X;

  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(tictactoe), on_draw, NULL, NULL);

  gesture = gtk_gesture_multi_press_new();
  g_signal_connect(gesture, "pressed", G_CALLBACK(on_button_pressed_cb), tictactoe);
  gtk_widget_add_controller(GTK_WIDGET(tictactoe), GTK_EVENT_CONTROLLER(gesture));

  g_signal_connect(G_OBJECT(tictactoe), "mark-added",
                   G_CALLBACK(mark_added_cb), NULL);
}

GtkWidget * 
gtk_tictactoe_new ()
{
  GtkTicTacToe *tictactoe;
  tictactoe = g_object_new (GTK_TYPE_TICTACTOE, NULL);  

  return GTK_WIDGET (tictactoe);
}

void
gtk_tictactoe_mark(GtkTicTacToe *tictactoe, guint x, guint y, GtkTicTacToeMark mark)
{
  if((x < 0) || (x > 2))
    {
      g_warning("Value of x must be between 0 and 2.");
      return;
    }

  if((y < 0) || (y > 2))
    {
      g_warning("Value of y must be between 0 and 2.");
      return;
    }

  tictactoe->data[x][y] = mark;
      
  if(tictactoe->current_mark == GTK_TICTACTOE_MARK_X)
    tictactoe->current_mark = GTK_TICTACTOE_MARK_O;
  else
    tictactoe->current_mark = GTK_TICTACTOE_MARK_X;

  gtk_widget_queue_draw(GTK_WIDGET(tictactoe));
  g_signal_emit(tictactoe, gtk_tictactoe_signals[MARK_ADDED], 0, x, y, mark);
}
