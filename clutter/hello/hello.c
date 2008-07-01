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
#include <clutter/clutter.h>
#include <clutter-gtk/gtk-clutter-embed.h>

#define STAGE_WIDTH 200
#define STAGE_HEIGHT 200

gboolean going_down;
gboolean going_right;

void
on_timeline_new_frame(ClutterTimeline *timeline,
                      gint frame_num,
                      gpointer data)
{
  ClutterActor *label_actor = CLUTTER_ACTOR(data);
  gint x, y, actor_width, actor_height;
  gint stage_rel_width, stage_rel_height;

  clutter_actor_get_position(label_actor, &x, &y);
  clutter_actor_get_size(label_actor, &actor_width, &actor_height);

  stage_rel_width = STAGE_WIDTH - actor_width;
  stage_rel_height = STAGE_HEIGHT - actor_height;

  if(x == 0)
    going_right = TRUE;
  else if(x == stage_rel_width)
    going_right = FALSE;

  if(y == 0)
    going_down = TRUE;
  else if(y == stage_rel_height)
    going_down = FALSE;

  if(going_down)
    y += 1;
  else
    y -= 1;

  if(going_right)
    x += 1;
  else
    x -= 1;

  clutter_actor_set_position(label_actor, x, y);
}

int main(int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *stage;
  ClutterActor *stage_actor;
  ClutterActor *label_actor;
  ClutterColor stage_color = { 0x00, 0x00, 0x00, 0xff };
  ClutterColor label_color = { 0xff, 0xff, 0xff, 0xff };
  ClutterTimeline *timeline;

  going_down = going_right = TRUE;
  
  clutter_init(&argc, &argv);
  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(G_OBJECT(window), "delete-event",
                   G_CALLBACK(gtk_main_quit), NULL);

  stage = gtk_clutter_embed_new();
  gtk_widget_set_size_request(stage, STAGE_WIDTH, STAGE_HEIGHT);
  gtk_container_add(GTK_CONTAINER(window), stage);

  stage_actor = gtk_clutter_embed_get_stage(GTK_CLUTTER_EMBED(stage));
  clutter_stage_set_color(CLUTTER_STAGE(stage_actor), &stage_color);
  
  label_actor = clutter_label_new_full("Bitstream Vera Sans",
                                       "Hello World!",
                                       &label_color);
  clutter_container_add_actor(CLUTTER_CONTAINER(stage_actor), label_actor);
  clutter_actor_show(label_actor);

  gtk_widget_show_all(window);

  timeline = clutter_timeline_new(10, 100);
  g_signal_connect(G_OBJECT(timeline), "new-frame",
                   G_CALLBACK(on_timeline_new_frame),
                   (gpointer)label_actor);
  clutter_timeline_set_loop(timeline, TRUE);
  clutter_timeline_start(timeline);

  gtk_main();

  return 0;
}
