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

#ifndef __GTKTICTACTOE_H__
#define __GTKTICTACTOE_H__ 1

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GTK_TYPE_TICTACTOE_MARK		  (gtk_tictactoe_mark_get_type ())

typedef enum
  {
    GTK_TICTACTOE_MARK_X = 1,
    GTK_TICTACTOE_MARK_O
  } GtkTicTacToeMark;


#define GTK_TYPE_TICTACTOE		  (gtk_tictactoe_get_type ())
#define GTK_TICTACTOE(obj)		  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TICTACTOE, GtkTicTacToe))
#define GTK_TICTACTOE_CLASS(klass)	  (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TICTACTOE, GtkTicTacToeClass))
#define GTK_IS_TICTACTOE(obj)	  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TICTACTOE))
#define GTK_IS_TICTACTOE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TICTACTOE))
#define GTK_TICTACTOE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TICTACTOE, GtkTicTacToeClass))
       

typedef struct _GtkTicTacToe       GtkTicTacToe;
typedef struct _GtkTicTacToeClass  GtkTicTacToeClass;

struct _GtkTicTacToe
{
  GtkDrawingArea parent;

  guint data[3][3];
};

struct _GtkTicTacToeClass
{
  GtkDrawingAreaClass parent_class;
};

GType gtk_tictactoe_get_type (void) G_GNUC_CONST;
GtkWidget* gtk_tictactoe_new ();
void gtk_tictactoe_mark(GtkTicTacToe *tictactoe, guint x, guint y, GtkTicTacToeMark mark);

G_END_DECLS

#endif
