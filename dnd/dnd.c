#include <gtk/gtk.h>

int main(int argc, char** argv)
{
    GtkWidget* window;
    GtkWidget* drawing_area;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
