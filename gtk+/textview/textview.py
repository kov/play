#!/usr/bin/env python
from gi.repository import Gtk

window = Gtk.Window()
window.connect('delete-event', Gtk.main_quit)

textview = Gtk.TextView()
window.add(textview)

buffer = textview.get_buffer()
buffer.set_text('Debian\n')

tag = Gtk.TextTag()
tag.set_property('weight', 400)
tag.set_property('weight-set', True)

tagtable = buffer.get_tag_table()
tagtable.add(tag)

buffer.apply_tag(tag, buffer.get_start_iter(), buffer.get_end_iter())

window.show_all()

Gtk.main()

