#include <gtk/gtk.h>

int main(int argc, char** argv)
{
  GtkWidget* window;
  GtkWidget* darea;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "delete-event", gtk_main_quit, NULL);

  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
