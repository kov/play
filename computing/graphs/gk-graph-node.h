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

#ifndef __GK_GRAPH_NODE_H__
#define __GK_GRAPH_NODE_H__ 1

#include <glib-object.h>

G_BEGIN_DECLS

#define GK_TYPE_GRAPH_NODE (gk_graph_node_get_type ())
#define GK_GRAPH_NODE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GK_TYPE_GRAPH_NODE, GkGraphNode))
#define GK_GRAPH_NODE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GK_TYPE_GRAPH_NODE, GkGraphNodeClass))
#define GK_IS_GRAPH_NODE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GK_TYPE_GRAPH_NODE))
#define GK_IS_GRAPH_NODE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GK_TYPE_GRAPH_NODE))
#define GK_GRAPH_NODE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GK_TYPE_GRAPH_NODE, GkGraphNodeClass))
       
typedef struct _GkGraphNode       GkGraphNode;
typedef struct _GkGraphNodeClass  GkGraphNodeClass;

struct _GkGraphNode
{
  GObject parent;

  glong id;

  GSList *neighbours;
};

struct _GkGraphNodeClass
{
  GObjectClass parent_class;
};

GType gk_graph_node_get_type (void) G_GNUC_CONST;

GkGraphNode* gk_graph_node_new(glong id);

glong gk_graph_node_get_id(GkGraphNode *self);

void gk_graph_node_add_neighbour(GkGraphNode *self, GkGraphNode *other, glong weight);

G_END_DECLS

#endif
