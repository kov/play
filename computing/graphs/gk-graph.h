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

#ifndef __GK_GRAPH_H__
#define __GK_GRAPH_H__ 1

#include <glib-object.h>
#include "gk-graph-node.h"

G_BEGIN_DECLS

#define GK_TYPE_GRAPH (gk_graph_get_type ())
#define GK_GRAPH(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GK_TYPE_GRAPH, GkGraph))
#define GK_GRAPH_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GK_TYPE_GRAPH, GkGraphClass))
#define GK_IS_GRAPH(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GK_TYPE_GRAPH))
#define GK_IS_GRAPH_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GK_TYPE_GRAPH))
#define GK_GRAPH_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GK_TYPE_GRAPH, GkGraphClass))
       
typedef struct _GkGraph       GkGraph;
typedef struct _GkGraphClass  GkGraphClass;

struct _GkGraph
{
  GObject parent;

  GHashTable *nodes;
};

struct _GkGraphClass
{
  GObjectClass parent_class;
};

GType gk_graph_get_type (void) G_GNUC_CONST;

GkGraph* gk_graph_new_from_g_file(GFile *file);

const GkGraphNode* gk_graph_get_node_by_id(GkGraph *graph, glong *node);

void gk_graph_add_node(GkGraph *graph, GkGraphNode *node);

void gk_graph_connect_nodes(GkGraph *graph, GkGraphNode *node1, GkGraphNode *node2, glong weight);

void gk_graph_add_path(GkGraph *graph, glong node, glong target_node, glong weight);

G_END_DECLS

#endif
