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

using GLib;
using Gee;
using Soup;

static const string base_uri = "http://www.giantitp.com";
static const string uri = "http://www.giantitp.com/comics/oots.html";

string get_page_data(string uri)
{
	URI soup_uri = new URI(uri);
	Message soup_message = new Message.from_uri("GET", soup_uri);
	Session soup_session = new SessionSync();

	soup_session.send_message(soup_message);

	weak Buffer soup_buffer = soup_message.response_body.flatten();
	string retval = soup_buffer.data;
	return retval;
}

string get_image_for_uri(string uri)
{
	string data = get_page_data(uri);
	if(data.has_prefix("<HTML"))
		stdout.printf("lala\n");
	return "a";
}

ArrayList<string>? get_comic_links_from_data(string data)
{
	ArrayList<string> result = new ArrayList<string>();
	Regex regex;
	MatchInfo minfo;

	try
	{
		regex = new Regex("/comics/oots[^\"]+");
	}
	catch (RegexError ex)
	{
		stderr.printf("Problem creating regexp: %s\n", ex.message);
		return null;
	}

	int count = 0;
	regex.match(data, 0, out minfo);
	while(minfo.matches() && (count < 10))
	{
		string href = minfo.fetch(0);
		result.add(href);
		count++;

		try
		{
			minfo.next();
		}
		catch (RegexError ex)
		{
			stderr.printf("Error trying next match: %s\n", ex.message);
			break;
		}
	}

	ArrayList<string> reversed_result = new ArrayList<string>();
	for(count = result.size - 1; count >= 0; count--)
		reversed_result.add(result.get(count));
	return reversed_result;
}

void main(string[] args)
{
	Thread.init(null);

	string data = get_page_data(uri);
	ArrayList<string> links = get_comic_links_from_data(data);
	
}