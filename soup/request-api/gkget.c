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
#define LIBSOUP_USE_UNSTABLE_REQUEST_API
#include <libsoup/soup.h>
#include <libsoup/soup-requester.h>
#include <stdio.h>


SoupSession* session;
SoupRequester* requester;

void gk_get_file(const char* uri)
{
    GError* error = NULL;
    SoupRequest* request = soup_requester_request(requester, uri, &error);

    if (error) {
        g_warning("Requesting uri `%s' failed: %s", uri, error->message);
        g_clear_error(&error);
        return;
    }

    GInputStream* istream = soup_request_send(request, NULL, &error);
    if (error) {
        g_warning("Requesting uri `%s' failed: %s", uri, error->message);
        g_object_unref(request);
        g_clear_error(&error);
        return;
    }

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

    session = soup_session_async_new();

    requester = soup_requester_new();
    soup_session_add_feature(session, SOUP_SESSION_FEATURE(requester));

    for (int i = 1; i < argc; i++)
        gk_get_file(argv[i]);

    return 0;
}
