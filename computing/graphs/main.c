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

#include <glib.h>
#include <gio/gio.h>

#include "gk-graph.h"
#include "gk-graph-node.h"
#include "gk-graph-edge.h"

void
show_one_connection(gpointer data, gpointer user_data)
{
  GkGraphEdge *edge = GK_GRAPH_EDGE(data);
  GkGraphNode *node = GK_GRAPH_NODE(user_data);
  g_message("Node %d connects to %d with a weight of %d!", node->id, edge->other->id, edge->weight);
}

void
show_connections(gpointer key, gpointer value, gpointer user_data)
{
  GkGraphNode *node = GK_GRAPH_NODE(value);
  
  g_slist_foreach(node->neighbours, show_one_connection, node);
}

int main()
{
  GkGraph *graph;
  GFile *file;

  g_type_init();

  file = g_file_new_for_path("teste2.txt");
  graph = gk_graph_new_from_g_file(file);

  g_hash_table_foreach(graph->nodes, show_connections, NULL);

  return 0;
}
