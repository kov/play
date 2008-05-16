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

#include <glib-object.h>

#include "gk-graph-node.h"
#include "gk-graph-edge.h"

G_DEFINE_TYPE(GkGraphNode, gk_graph_node, G_TYPE_OBJECT);

static void
gk_graph_node_finalize (GObject *object)
{
  G_OBJECT_CLASS (gk_graph_node_parent_class)->finalize (object);
}

static void
gk_graph_node_dispose (GObject *object)
{
  GkGraphNode *graph_node = GK_GRAPH_NODE(object);
  g_slist_free(graph_node->neighbours);
}

static void
gk_graph_node_class_init (GkGraphNodeClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = gk_graph_node_dispose;
  object_class->finalize = gk_graph_node_finalize;
}

static void
gk_graph_node_init (GkGraphNode *graph_node)
{
  graph_node->neighbours = NULL;
}

GkGraphNode*
gk_graph_node_new(glong id)
{
  GkGraphNode *graph_node = GK_GRAPH_NODE(g_object_new(GK_TYPE_GRAPH_NODE, NULL));
  /* should be turned into a property! */
  graph_node->id = id;
  return graph_node;
}

glong
gk_graph_node_get_id(GkGraphNode *self)
{
  return self->id;
}

void
gk_graph_node_add_neighbour(GkGraphNode *self, GkGraphNode *other, glong weight)
{
  GSList *element;
  GkGraphEdge *graph_edge = gk_graph_edge_new(other, weight);
  
  /* tries to find if the element is already on the neighbours list */
  element = g_slist_find(self->neighbours, graph_edge);
  if(element != NULL)
    {
      g_object_unref(graph_edge);
      return;
    }

  self->neighbours = g_slist_prepend(self->neighbours, graph_edge);
}
