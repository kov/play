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

#include "gk-graph-edge.h"

G_DEFINE_TYPE(GkGraphEdge, gk_graph_edge, G_TYPE_OBJECT);

static void
gk_graph_edge_finalize (GObject *object)
{
  G_OBJECT_CLASS (gk_graph_edge_parent_class)->finalize (object);
}

static void
gk_graph_edge_dispose (GObject *object)
{
  GkGraphEdge *graph_edge = GK_GRAPH_EDGE(object);
  g_object_unref(graph_edge->other);
}

static void
gk_graph_edge_class_init (GkGraphEdgeClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = gk_graph_edge_dispose;
  object_class->finalize = gk_graph_edge_finalize;
}

static void
gk_graph_edge_init (GkGraphEdge *graph_edge)
{
}

GkGraphEdge*
gk_graph_edge_new(GkGraphNode *other, glong weight)
{
  GkGraphEdge *graph_edge = GK_GRAPH_EDGE(g_object_new(GK_TYPE_GRAPH_EDGE, NULL));

  /* should be turned into a property! */
  g_object_ref(other);
  graph_edge->other = other;
  graph_edge->weight = weight;

  return graph_edge;
}

GkGraphNode*
gk_graph_edge_get_other_node(GkGraphEdge *graph_edge, glong *weight)
{
  *weight = graph_edge->weight;
  return graph_edge->other;
}
