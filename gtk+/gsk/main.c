#include <gtk/gtk.h>

int main(int argc, char **argv) {
    gtk_init();

    GdkSurface *surface = gdk_surface_new_toplevel(gdk_display_get_default(), 1024, 768);
    gdk_surface_show(surface);

    GskRenderer *renderer = gsk_renderer_new_for_surface(surface);

    // A black background.
    GdkRGBA color;
    gdk_rgba_parse(&color, "black");

    graphene_rect_t bounds;
    graphene_rect_init(&bounds, 0, 0, 1024, 768);

    GskRenderNode *bg_node = gsk_color_node_new(&color, &bounds);

    // A red rectangle that we want...
    gdk_rgba_parse(&color, "red");
    graphene_rect_init(&bounds, 0, 0, 800, 600);
    GskRenderNode *red_node = gsk_color_node_new(&color, &bounds);

    // ...in the middle of the screen, and rotated slightly...
    // Would actually need to properly setup projection matrix here.
    graphene_matrix_t transform;
    graphene_matrix_init_identity(&transform);

    graphene_point3d_t offset;
    graphene_point3d_init(&offset, (float)(1024 - 800) / 2., (float)(768 - 600) / 2., 0.);

    graphene_matrix_translate(&transform, &offset);
    graphene_matrix_rotate_y(&transform, 30);
    graphene_matrix_rotate_x(&transform, 10);

    red_node = gsk_transform_node_new(red_node, &transform);

    // Both need to be put on a container...
    GskRenderNode **children = g_newa(GskRenderNode *, 2);
    children[0] = bg_node;
    children[1] = red_node;
    
    GskRenderNode *node = gsk_container_node_new(children, 2);

    gsk_renderer_render(renderer, node, NULL);

    sleep(5);

    return 0;
}