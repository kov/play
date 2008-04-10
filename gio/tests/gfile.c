/*
 * Copyright (C) 2008 Gustavo Noronha Silva
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

#include <stdio.h>
#include <gio/gio.h>

int main()
{
    GFile *file;
    GFileInputStream *input;
    GFileOutputStream *output;
    GError *error = NULL;
    gchar *buffer = g_new0(gchar, 10);

    g_type_init();

    file = g_file_new_for_uri("file:///home/kov/src/gio/tests/teste.txt");
    input = g_file_read(file, NULL, &error);
    if(error)
    {
        printf ("erro: %s\n", error->message);
        g_error_free(error);
    }
    output = g_file_append_to(file, G_FILE_CREATE_NONE, NULL, NULL);

    g_input_stream_read_all(G_INPUT_STREAM(input), buffer, 10, NULL, NULL, NULL);
    printf("buffer: %s\n", buffer);
    g_seekable_seek(G_SEEKABLE(input), 0, G_SEEK_END, NULL, NULL);
    g_output_stream_write_all(G_OUTPUT_STREAM(output), "lerolerole", 10, NULL, NULL, NULL);
    g_input_stream_read_all(G_INPUT_STREAM(input), buffer, 10, NULL, NULL, NULL);
    printf("buffer: %s\n", buffer);

    return 0;
}
