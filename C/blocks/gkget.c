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


#include <Block.h>
#include <glib.h>
#define LIBSOUP_USE_UNSTABLE_REQUEST_API
#include <libsoup/soup.h>
#include <libsoup/soup-requester.h>
#include <stdio.h>

GMainLoop* loop;
SoupSession* session;
SoupRequester* requester;

typedef void (^Block)();

static void async_result_cb(GObject* source,
                            GAsyncResult* res,
                            gpointer data)
{
    Block block = (Block)data;
    block(res);
}

void gk_get_file(const char* uri)
{
    __block GError* error = NULL;
    SoupRequest* request = soup_requester_request(requester, uri, &error);

    if (error) {
        g_warning("Requesting uri `%s' failed: %s", uri, error->message);
        g_clear_error(&error);
        return;
    }

    __block GInputStream* stream;
    soup_request_send_async(request, NULL, async_result_cb, (gpointer) ^ (GAsyncResult* res) {
            stream = soup_request_send_finish(request, res, &error);
            if (error) {
                g_warning("Error: %s", error->message);
                g_clear_error(&error);
                return;
            }

            __block char* buffer = (char*)g_malloc(4096);

            Block read_block = ^ (GAsyncResult* res) {
                gssize nread = g_input_stream_read_finish(stream, res, &error);
                if (error) {
                    g_warning("Error reading: %s", error->message);
                    g_clear_error(&error);
                    return;
                }

                if (nread) {
                    char* buf = buffer;
                    gssize written = 0;
                    do {
                        written += write(1, buf + written, nread - written);
                    } while (written < nread);

                    g_input_stream_read_async(stream, buffer, 4096, G_PRIORITY_DEFAULT,
                                              NULL, async_result_cb, read_block);
                } else
                    g_free(buffer);
            };

            g_input_stream_read_async(stream, buffer, 4096, G_PRIORITY_DEFAULT,
                                      NULL, async_result_cb, read_block);

            g_object_unref(stream);
        });

    g_object_unref(request);
}

int main(int argc, char** argv)
{
    g_type_init();

    session = soup_session_async_new();

    requester = soup_requester_new();
    soup_session_add_feature(session, SOUP_SESSION_FEATURE(requester));

    loop = g_main_loop_new(NULL, TRUE);

    for (int i = 1; i < argc; i++)
        gk_get_file(argv[i]);

    g_main_loop_run(loop);

    return 0;
}
