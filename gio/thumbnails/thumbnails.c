#include <stdio.h>
#include <gio/gio.h>

int main(int argc, char** argv)
{
    if (argc < 2)
        return 1;

    g_type_init();

    GFile* file = g_file_new_for_path(argv[1]);
    GFileInfo* info = g_file_query_info(file, "thumbnail::*", 0, NULL, NULL);
    const char* thumbnail_path = g_file_info_get_attribute_string(info, G_FILE_ATTRIBUTE_THUMBNAIL_PATH);
    gboolean error_generating = g_file_info_get_attribute_boolean(info, G_FILE_ATTRIBUTE_THUMBNAILING_FAILED);
    printf("Thumbnail: %s [%d]\n", thumbnail_path, error_generating);
    return 0;
}

