#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright (©) 2012 Gustavo Noronha Silva
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.  You should have received a copy of the GNU General Public
# License along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.

import gc
import os
from gi.repository import GLib, Gio


main_loop = GLib.MainLoop()
count = 0
scan_queue = []
scanned_directories = []

def callback(directory, result, path):
    global count

    GLib.idle_add(lazy_list_directory)
    try:
        enumerator = directory.enumerate_children_finish(result)
    except GLib.GError, e:
        # Permission denied
        if e.code == 14:
            return

        # File not found
        if e.code ==1:
            return

        raise GLib.GError(e)

    if path.startswith('/proc') or path.startswith('/sys'):
        return

    for info in enumerator:
        name = os.path.join(path, info.get_attribute_byte_string('standard::name'))
        count = count + 1
        print name, count
        if info.get_attribute_uint32('standard::type') == Gio.FileType.DIRECTORY.real:
            if info.get_attribute_boolean('standard::is-symlink'):
                target = info.get_attribute_byte_string('standard::symlink-target')
                if target not in scanned_directories:
                    scan_queue.append(target)
            else:
                scan_queue.append(name)

    enumerator.close(None)
    del enumerator
    del directory
    gc.collect()


def lazy_list_directory(*args):
    if scan_queue:
        list_directory(scan_queue.pop(0))
        return False
    return True


def list_directory(path):
    scanned_directories.append(path)
    directory = Gio.file_new_for_path(path)
    directory.enumerate_children_async('standard::*', 0, 0, None, callback, path)


list_directory('/')
main_loop.run()
