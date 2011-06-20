#!/usr/bin/env seed

Gtk = imports.gi.Gtk;
WebKit = imports.gi.WebKit;

Gtk.init(Seed.argv);

var window = new Gtk.Window();
window.set_default_size(800, 600);

var scrolled_window = new Gtk.ScrolledWindow();
window.add(scrolled_window);

var web_view = new WebKit.WebView();
scrolled_window.add(web_view);

web_view.load_uri('http://debian.org/');

window.show_all();

Gtk.main();
