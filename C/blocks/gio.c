#include <Block.h>
#include <gio/gio.h>

typedef void (^Block)();

static void async_result_cb(GObject* source,
                            GAsyncResult* res,
                            gpointer data)
{
    Block block = (Block)data;
    block(res);
}

int main(int argc, char** argv)
{
    g_type_init();

    if (argc != 2) {
        g_error("Blah.");
        return 1;
    }

    GMainLoop* loop = g_main_loop_new(NULL, TRUE);
    GFile* file = g_file_new_for_path(argv[1]);

    g_file_query_info_async(file,
                            G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE,
                            G_FILE_QUERY_INFO_NONE, G_PRIORITY_DEFAULT,
                            NULL, async_result_cb, (gpointer) ^ (GAsyncResult* res) {
        GError* error = NULL;
        GFileInfo* info = g_file_query_info_finish(file, res, &error);

        if (error) {
            g_error("Failed: %s", error->message);
            g_error_free(error);
            return;
        }

        g_message("Content Type: %s",
                  g_file_info_get_attribute_string(info, G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE));

        g_object_unref(info);
        g_main_loop_quit(loop);
    });

    g_main_loop_run(loop);
    g_object_unref(file);

    return 0;
}
