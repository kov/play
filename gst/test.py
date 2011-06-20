#!/usr/bin/python

import gst
import gtk

class Application(object):
    def __init__(self):
        self.main_window = gtk.Window()
        self.main_window.set_default_size(640, 480)

        vbox = gtk.VBox(False, 2)
        self.main_window.add(vbox)

        self.video = gtk.DrawingArea()
        vbox.pack_start(self.video, expand = True, fill = True)

        button_box = gtk.HButtonBox()
        vbox.pack_start(button_box, expand = False, fill = True)

        self.play_button = gtk.Button(gtk.STOCK_MEDIA_PAUSE)
        self.play_button.connect('clicked', self.on_play_button_click)
        self.play_button.set_property('use_stock', True)
        button_box.add(self.play_button)

        self.pipeline = gst.Pipeline('pipeline')
        self.playbin = gst.element_factory_make('playbin2', 'playbin')

        self.playbin_bus = self.playbin.get_bus()
        self.playbin_bus.add_signal_watch()
        self.playbin_bus.enable_sync_message_emission()
        self.playbin_bus.connect('message', self.on_message)
        self.playbin_bus.connect('sync-message::element', self.on_sync_message)

        self.main_window.show_all()

    def play_uri(self, uri):
        self.playbin.set_state(gst.STATE_NULL)
        self.playbin.set_property('uri', uri)
        self.playbin.set_state(gst.STATE_PLAYING)

    def on_play_button_click(self, widget):
        state = self.playbin.get_state()[1]

        if state != gst.STATE_PLAYING:
            self.playbin.set_state(gst.STATE_PLAYING)
            self.play_button.set_label(gtk.STOCK_MEDIA_PAUSE)
            return

        self.playbin.set_state(gst.STATE_PAUSED)
        self.play_button.set_label(gtk.STOCK_MEDIA_PLAY)

    def on_message(self, bus, message):
        if message.type == gst.MESSAGE_ERROR or \
                message.type == gst.MESSAGE_EOS:
            self.playbin.set_state(gst.STATE_NULL)
            gtk.gdk.threads_enter()
            self.play_button.set_label(gtk.STOCK_MEDIA_PLAY)
            gtk.gdk.threads_leave()
        elif message.type == gst.MESSAGE_BUFFERING:
            state = self.playbin.get_state()[1]

            if state == gst.STATE_PLAYING:
                self.playbin.set_state(gst.STATE_PAUSED)
                self.play_button.set_label('Buffering...')
                self.play_button.set_sensitive(False)
                return

            percentage = message.structure['buffer-percent']
            self.play_button.set_label('Buffering... (%d)' % percentage)
            if percentage == 100:
                self.playbin.set_state(gst.STATE_PLAYING)
                self.play_button.set_label(gtk.STOCK_MEDIA_PAUSE)
                self.play_button.set_sensitive(True)

        if message.type == gst.MESSAGE_ERROR:
            error, debug = message.parse_error()
            print error, debug

    def on_sync_message(self, bus, message):
        if message.structure is None:
            return

        if message.structure.get_name() == 'prepare-xwindow-id':
            sink = message.src
            sink.set_property('force-aspect-ratio', True)
            gtk.gdk.threads_enter()
            sink.set_xwindow_id(self.video.window.xid)
            gtk.gdk.threads_leave()

if __name__ == '__main__':
    app = Application()
    app.play_uri('http://blip.tv/file/get/Notmart-plasmamobilevertical118.ogv')
    gtk.gdk.threads_init()
    gtk.main()
