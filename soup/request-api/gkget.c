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
#include <stdlib.h>

GMainLoop* loop;
SoupSession* session;
SoupRequester* requester;
char* buffer;

void async_read_cb(GObject* source, GAsyncResult* res, gpointer data)
{
    GInputStream* stream = G_INPUT_STREAM(source);
    GError* error = NULL;
    gssize nread = g_input_stream_read_finish(stream, res, &error);

    if (error) {
        g_warning("Reading failed: %s", error->message);
        g_clear_error(&error);
        return;
    }

    if (!nread) {
        g_main_loop_quit(loop);
        return;
    }

    write(1, buffer, nread);

    g_input_stream_read_async(stream, buffer, 4096, G_PRIORITY_DEFAULT, NULL, async_read_cb, NULL);
}

void async_send_cb(GObject* source, GAsyncResult* res, gpointer data)
{
    SoupRequest* request = SOUP_REQUEST(source);
    GError* error = NULL;
    GInputStream* stream = soup_request_send_finish(request, res, &error);
    if (error) {
        g_warning("Requesting uri failed", error->message);
        g_object_unref(request);
        g_clear_error(&error);
        return;
    }

    g_input_stream_read_async(stream, buffer, 4096, G_PRIORITY_DEFAULT, NULL, async_read_cb, NULL);
}

void gk_get_file(const char* uri)
{
    GError* error = NULL;
    SoupRequest* request = soup_requester_request(requester, uri, &error);

    if (error) {
        g_warning("Requesting uri `%s' failed: %s", uri, error->message);
        g_clear_error(&error);
        return;
    }

    soup_request_send_async(request, NULL, async_send_cb, NULL);
}

int main(int argc, char** argv)
{
    g_type_init();

    buffer = (char*)malloc(4096);

    session = soup_session_async_new();

    requester = soup_requester_new();
    soup_session_add_feature(session, SOUP_SESSION_FEATURE(requester));

    loop = g_main_loop_new(NULL, TRUE);

    gk_get_file(argv[1]);

    g_main_loop_run(loop);

    return 0;
}
