#include <gtk/gtk.h>

void expose_event_cb(GtkWidget* widget, GdkEventExpose* event, gpointer data)
{
    cairo_t* cr = gdk_cairo_create(widget->window);

    cairo_save(cr);

    // uncommenting this makes it not work
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);

    cairo_move_to(cr, 10.0, 0.0);
    cairo_line_to(cr, 0.0, 480.0);
    cairo_line_to(cr, 640.0, 480.0);
    cairo_line_to(cr, 640.0, 0.0);
    cairo_close_path(cr);

    cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.5);
    cairo_fill_preserve(cr);

    cairo_restore(cr);
}

int main(int argc, char** argv)
{
    GtkWidget* window;
    GtkWidget* drawing_area;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    drawing_area = gtk_drawing_area_new();
    g_signal_connect(drawing_area, "expose-event",
                     G_CALLBACK(expose_event_cb), NULL);

    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
