#include <gio/gio.h>
#include <gtk/gtk.h>

#define LIBSOUP_USE_UNSTABLE_REQUEST_API
#include <libsoup/soup-multipart-input-stream.h>
#include <libsoup/soup-request-http.h>
#include <libsoup/soup-requester.h>
#include <libsoup/soup.h>

#define READ_BLOCK_SIZE 8096

GtkBuilder* builder;
GdkPixbufLoader* loader;
char buffer[READ_BLOCK_SIZE];

static void update_image(SoupMultipartInputStream* multipart);
static void close_cb (GObject* source, GAsyncResult* res, gpointer data);
static void read_cb (GObject* source, GAsyncResult* res, gpointer data);
static void next_part_cb (GObject* source, GAsyncResult* res, gpointer data);
static void request_sent_cb (GObject* source, GAsyncResult* res, gpointer data);

static void
update_image(SoupMultipartInputStream* multipart)
{
    GtkWindow* main_window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    gtk_window_present(main_window);

    GError* error = NULL;
    gdk_pixbuf_loader_close(loader, &error);
    if (error) {
        g_warning("Error closing loader: %s", error->message);
        return;
    }

    GdkPixbuf* pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);
    GtkImage* image = GTK_IMAGE(gtk_builder_get_object(builder, "image"));
    gtk_image_set_from_pixbuf(image, pixbuf);

    g_object_unref(loader);
    loader = NULL;
}

static void
close_cb (GObject* source, GAsyncResult* res, gpointer data)
{
    g_input_stream_close_finish(G_INPUT_STREAM(source), res, NULL);
}

static void
read_cb (GObject* source, GAsyncResult* res, gpointer data)
{
    GInputStream* in = G_INPUT_STREAM(source);
    GError* error;

    gssize nread = g_input_stream_read_finish(in, res, &error);

    if (error)
        g_warning ("Error reading: %s", error->message);

    if (nread <= 0) {
        g_input_stream_close_async(in, G_PRIORITY_DEFAULT, NULL, close_cb, NULL);

        SoupMultipartInputStream* multipart = SOUP_MULTIPART_INPUT_STREAM(data);

        update_image(multipart);

        soup_multipart_input_stream_next_part_async(multipart, G_PRIORITY_DEFAULT, NULL, next_part_cb, NULL);
        return;
    }

    gdk_pixbuf_loader_write(loader, (const guchar*)buffer, nread, &error);
    if (error) {
        g_warning("Error writing to loader: %s", error->message);
        return;
    }

    g_input_stream_read_async(in, buffer, READ_BLOCK_SIZE, G_PRIORITY_DEFAULT, NULL, read_cb, data);
}

static void
next_part_cb (GObject* source, GAsyncResult* res, gpointer data)
{
    SoupMultipartInputStream* multipart = SOUP_MULTIPART_INPUT_STREAM(source);

    GInputStream* in = soup_multipart_input_stream_next_part_finish(multipart, res, NULL);
    if (!in) {
        g_input_stream_close_async(G_INPUT_STREAM(multipart), G_PRIORITY_DEFAULT, NULL, close_cb, NULL);
        return;
    }

    g_assert(!loader);

    SoupMessageHeaders* headers = soup_multipart_input_stream_get_headers(multipart);
    const char* content_type = soup_message_headers_get_content_type(headers,  NULL);
    loader = gdk_pixbuf_loader_new_with_mime_type(content_type, NULL);

    g_input_stream_read_async(in, buffer, READ_BLOCK_SIZE, G_PRIORITY_DEFAULT, NULL, read_cb, multipart);
}

static void
request_sent_cb (GObject* source, GAsyncResult* res, gpointer data)
{
    SoupRequest* request = SOUP_REQUEST(source);
    GInputStream* stream = soup_request_send_finish(request, res, NULL);

    SoupMessage* message = soup_request_http_get_message(SOUP_REQUEST_HTTP(request));
    SoupMultipartInputStream* multipart = soup_multipart_input_stream_new(message, stream);

    soup_multipart_input_stream_next_part_async(multipart, G_PRIORITY_DEFAULT, NULL, next_part_cb, NULL);
}

int main(int argc, char** argv)
{
    gtk_init(&argc, &argv);

    if (argc < 2) {
        g_error("Usage: %s <uri>", argv[0]);
        return 1;
    }

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "mjpeg-viewer.ui", NULL);

    SoupSession* session = soup_session_async_new();
    soup_session_add_feature_by_type(session, SOUP_TYPE_REQUESTER);

    SoupRequester* requester = SOUP_REQUESTER(soup_session_get_feature(session, SOUP_TYPE_REQUESTER));
    SoupRequest* request = soup_requester_request(requester, argv[1], NULL);

    soup_request_send_async(request, NULL, request_sent_cb, NULL);

    gtk_main();
    return 0;
}
