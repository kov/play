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

#include <string.h>

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include "gk-graph.h"

G_DEFINE_TYPE(GkGraph, gk_graph, G_TYPE_OBJECT);

static void
gk_graph_finalize (GObject *object)
{
  G_OBJECT_CLASS (gk_graph_parent_class)->finalize (object);
}

static void
gk_graph_dispose (GObject *object)
{
  GkGraph *graph = GK_GRAPH(object);

  g_hash_table_destroy(graph->nodes);
}

static void
gk_graph_class_init (GkGraphClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = gk_graph_dispose;
  object_class->finalize = gk_graph_finalize;
}

static void
gk_graph_init (GkGraph *graph)
{
  /* the hash table will be indexed using integers, and hold GkGraphNode objects */
  graph->nodes = g_hash_table_new_full(g_int_hash, g_int_equal, g_free, g_object_unref);
}

static gint
str_find(gchar *haystack, gchar needle, gint len)
{
  gint count;

  for(count = 0; count < len; count++)
    {
      if(haystack[count] == needle)
        return count;
    }

  return -1;
}

static void
parse_line(GkGraph *graph, gchar *line)
{
  glong node, target_node, weight;
  gchar *endptr = NULL;

  node = strtol(line, &endptr, 10);

  while((*endptr != '\n') && (*endptr != '\0'))
    {
      if(*endptr != ' ')
        {
          g_warning("Invalid file format! Expecting ' ', got '%c'!", *endptr);
          g_free(line);
          g_object_unref(graph);
          return;
        }
      endptr++;
      target_node = strtol(endptr, &endptr, 10);
      if(*endptr != ';')
        {
          g_warning("Invalid file format! Expecting ';', got '%c'!", *endptr);
          g_free(line);
          g_object_unref(graph);
          return;
        }
      endptr++;
      weight = strtol(endptr, &endptr, 10);

      gk_graph_add_path(graph, node, target_node, weight);
    }
}

#define CHUNK_SIZE 256
GkGraph*
gk_graph_new_from_g_file(GFile *file)
{
  GkGraph *graph;
  GFileInputStream *input;
  GError *error = NULL;
  gchar *line = NULL;

  graph = g_object_new(GK_TYPE_GRAPH, NULL);
  input = g_file_read(file, NULL, &error);

  {
    gchar *line = NULL;
    gint line_size = 0;
    gint bytes_read = 0;
    gchar buffer[CHUNK_SIZE] = { 0 };
    GError *error = NULL;

    while(bytes_read = g_input_stream_read(G_INPUT_STREAM(input), buffer, CHUNK_SIZE, NULL, &error))
      {
        gint newline = 0;
        gchar *ptr = buffer;

        while((newline != -1) && bytes_read)
          {
            newline = str_find(ptr, '\n', CHUNK_SIZE);
            gint chars_to_append;

            if(error)
              {
                g_warning("Failed to read: %s", error->message);
                g_error_free(error);
                break;
              }

            if(newline != -1)
              chars_to_append = newline;
            else
              chars_to_append = bytes_read;

            line = g_realloc(line, sizeof(gchar) * (line_size + chars_to_append));
            memcpy(line + line_size, ptr, chars_to_append);
            line_size += chars_to_append;

            if(newline != -1)
              {
                /* make sure the line ends with a NULL char, if we actually have a line */
                if(line_size)
                  {
                    line = g_realloc(line, sizeof(gchar) * (line_size + 1));
                    line[line_size] = '\0';
                    parse_line(graph, line);
                  }
                g_free(line);
                line = NULL;
                line_size = 0;
              }

            ptr += chars_to_append + 1;
            bytes_read -= chars_to_append;
          }
      }
  }

  if(error)
    {
      g_warning("Could not open file %s: %s", g_file_get_uri(file), error->message);
      g_error_free(error);
      return NULL;
    }


  return graph;
}

const GkGraphNode*
gk_graph_get_node_by_id(GkGraph *graph, glong *node)
{
  GkGraphNode *node_obj = NULL;

  node_obj = g_hash_table_lookup(graph->nodes, node);
  return node_obj;
}

void
gk_graph_add_node(GkGraph *graph, GkGraphNode *node)
{
  glong *id = g_malloc(sizeof(glong));

  *id = gk_graph_node_get_id(node);
  g_hash_table_insert(graph->nodes, id, node);
}

void
gk_graph_connect_nodes(GkGraph *graph, GkGraphNode *node1, GkGraphNode *node2, glong weight)
{
  gk_graph_node_add_neighbour(node1, node2, weight);
  gk_graph_node_add_neighbour(node2, node1, weight);
}

void
gk_graph_add_path(GkGraph *graph, glong node1, glong node2, glong weight)
{
  GkGraphNode *node1_obj, *node2_obj;

  node1_obj = (GkGraphNode*)gk_graph_get_node_by_id(graph, &node1);
  if(node1_obj == NULL)
    {
      node1_obj = gk_graph_node_new(node1);
      gk_graph_add_node(graph, node1_obj);
    }

  node2_obj = (GkGraphNode*)gk_graph_get_node_by_id(graph, &node2);
  if(node2_obj == NULL)
    {
      node2_obj = gk_graph_node_new(node2);
      gk_graph_add_node(graph, node2_obj);
    }

  gk_graph_connect_nodes(graph, node1_obj, node2_obj, weight);
}
