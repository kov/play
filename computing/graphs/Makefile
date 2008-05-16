all: graph

graph: main.c gk-graph.c gk-graph.h gk-graph-node.c gk-graph-node.h gk-graph-edge.c gk-graph-edge.h
	gcc -g -o graph main.c gk-graph.c gk-graph-node.c gk-graph-edge.c `pkg-config --cflags --libs gobject-2.0 gio-2.0`
