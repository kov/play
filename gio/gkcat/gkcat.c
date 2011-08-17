/*
 * Copyright © 2011 Gustavo Noronha Silva
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.  You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 */

#include <glib.h>
#include <gio/gio.h>
#include <stdio.h>


static void gk_cat_file(const char* uri)
{
    GFile* file = g_file_new_for_uri(uri);

    GError* error = NULL;
    GFileInfo* info = g_file_query_info(file,
                                        G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE,
                                        G_FILE_QUERY_INFO_NONE,
                                        NULL,
                                        &error);

    if (error) {
        g_warning("Error opening `%s': %s", uri, error->message);
        g_clear_error(&error);
        g_object_unref(file);
        return;
    }

    gboolean is_gzip = !g_strcmp0("application/x-gzip",
                                  g_file_info_get_attribute_string(info,
                                                                   G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE));
    g_object_unref(info);

    GFileInputStream* file_istream = g_file_read(file, NULL, &error);
    g_object_unref(file);

    if (error) {
        g_warning("Error opening `%s': %s", uri, error->message);
        g_clear_error(&error);
        return;
    }

    GInputStream* istream;
    if (is_gzip) {
        GZlibDecompressor* decompressor = g_zlib_decompressor_new(G_ZLIB_COMPRESSOR_FORMAT_GZIP);
        istream = g_converter_input_stream_new(G_INPUT_STREAM(file_istream), G_CONVERTER(decompressor));
        g_object_unref(file_istream);
    } else
        istream = G_INPUT_STREAM(file_istream);

    char* buffer = g_malloc(1024);
    gsize bytes_read = -1;
    do {
        g_input_stream_read_all(istream,
                                buffer,
                                1024,
                                &bytes_read,
                                NULL,
                                &error);

        if (error) {
            g_warning("Error reading from file `%s': %s", uri, error->message);
            g_clear_error(&error);
            g_free(buffer);
            g_object_unref(istream);
            return;
        }

        fwrite(buffer, 1, bytes_read, stdout);
    } while (bytes_read != 0);

    g_free(buffer);
    g_object_unref(istream);
}

int main(int argc, char** argv)
{
    g_type_init();

    for (int i = 1; i < argc; i++)
        gk_cat_file(argv[i]);

    return 0;
}
