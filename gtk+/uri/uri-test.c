#include <gtk/gtk.h>
#include <time.h>

int main(int argc, char** argv)
{
    gtk_init(&argc, &argv);

    time_t start_time = time(NULL);
    GError* error = NULL;
    gtk_show_uri(gdk_screen_get_default(), "https://wiki.collabora.co.uk/", start_time, &error);
    printf("Took: %d seconds\n", time(NULL) - start_time);

    if (error) {
        printf("Error: %s\n", error->message);
        g_clear_error(&error);
    }

    return 0;
}
