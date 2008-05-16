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

#ifndef __GK_GRAPH_EDGE_H__
#define __GK_GRAPH_EDGE_H__ 1

#include <glib-object.h>
#include "gk-graph-node.h"

G_BEGIN_DECLS

#define GK_TYPE_GRAPH_EDGE (gk_graph_edge_get_type ())
#define GK_GRAPH_EDGE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GK_TYPE_GRAPH_EDGE, GkGraphEdge))
#define GK_GRAPH_EDGE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GK_TYPE_GRAPH_EDGE, GkGraphEdgeClass))
#define GK_IS_GRAPH_EDGE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GK_TYPE_GRAPH_EDGE))
#define GK_IS_GRAPH_EDGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GK_TYPE_GRAPH_EDGE))
#define GK_GRAPH_EDGE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GK_TYPE_GRAPH_EDGE, GkGraphEdgeClass))
       
typedef struct _GkGraphEdge       GkGraphEdge;
typedef struct _GkGraphEdgeClass  GkGraphEdgeClass;

struct _GkGraphEdge
{
  GObject parent;

  glong weight;

  GkGraphNode *other;
};

struct _GkGraphEdgeClass
{
  GObjectClass parent_class;
};

GType gk_graph_edge_get_type (void) G_GNUC_CONST;

GkGraphEdge* gk_graph_edge_new(GkGraphNode *other, glong weight);

GkGraphNode* gk_graph_edge_get_other_node(GkGraphEdge *graph_edge, glong *weight);

G_END_DECLS

#endif
