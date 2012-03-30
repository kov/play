#include <gtk/gtk.h>

int main(int argc, char** argv)
{
    gtk_init(&argc, &argv);
    
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget* textview = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(window), textview);

    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_buffer_set_text(buffer, "Debian\n", -1);
    
    GtkTextTag* tag = gtk_text_buffer_create_tag(buffer, "b", "background", "red", NULL);

    GtkTextIter start;
    gtk_text_buffer_get_start_iter(buffer, &start);


    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);

    gtk_text_buffer_apply_tag_by_name(buffer, "b", &start, &end);
    
    gtk_widget_show_all(window);
    gtk_main();
    
    return 0;
}

