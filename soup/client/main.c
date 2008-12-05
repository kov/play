/*
 * Test soup client
 *
 * Copyright (C) 2008 Gustavo Noronha Silva
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <libsoup/soup.h>

int main(int argc, char **argv)
{
  SoupSession *session;
  SoupCookieJar *cookie_jar;
  SoupMessage *message;

  g_type_init();
  g_thread_init(NULL);

  g_message("Creating session...");
  session = soup_session_sync_new();

  cookie_jar = soup_cookie_jar_text_new("/tmp/cookie_test.txt", FALSE);
  soup_session_add_feature(session, SOUP_SESSION_FEATURE(cookie_jar));

  message = soup_message_new("GET",
                             "http://google.com/");
  soup_session_send_message(session, message);

  g_message("Status: %d", message->status_code);

  g_message("Body:\n%s", message->response_body->data);

  return 0;
}
