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

[DBus (name = "br.eti.kov.GUptime")]
public class GUptime : Object {
	public double get_uptime() {
		return 1.0;
	}
}

int main() {
	MainLoop loop = new MainLoop(null, false);

	try {
		var conn = DBus.Bus.get(DBus.BusType. SESSION);
		
		dynamic DBus.Object bus = conn.get_object("org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus");

		uint request_name_result = bus.RequestName ("br.eti.kov.GUptime", (uint) 0);

		var server = new GUptime ();
		conn.register_object ("/br/eti/kov/guptime", server);

		loop.run ();
	} catch (Error foo) {
		stderr.printf("Oops %s\n", foo.message);
	}

	return 0;
}