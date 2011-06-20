#include <clutter/clutter.h>
#include <clutter-gst/clutter-gst.h>

gboolean bus_message_cb(GstBus* bus, GstMessage* message, gpointer data)
{
    GError *error = NULL;

    switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_ERROR:
        gst_message_parse_error(message, &error, NULL);
        g_debug("Error %d: %s", error->code,  error->message);
        break;
    default:
        g_debug("Unhandled message.");
    }
}

int main(int argc, char **argv)
{
    ClutterActor *stage;
    ClutterActor *video;
    GstElement *player;
    GError *error = NULL;
    GstBus *bus;

    gst_init_check(0, 0, &error);
    if (error) {
        g_error(error->message);
        return 1;
    }

    clutter_init(&argc, &argv);
    stage = clutter_stage_get_default();
    clutter_actor_set_size(stage, 800, 600);
    clutter_stage_set_user_resizable(CLUTTER_STAGE(stage), TRUE);

    video = clutter_gst_video_texture_new();

    player = clutter_gst_video_texture_get_pipeline(video);
    bus = gst_element_get_bus(player);
    g_signal_connect(bus, "message", G_CALLBACK(bus_message_cb), NULL);

    clutter_media_set_uri(CLUTTER_MEDIA(video), "file:///home/kov/WOWX3-Cataclysm_Trailer_enUS_ESRB.avi");
    clutter_media_set_playing(CLUTTER_MEDIA(video), TRUE);

    clutter_container_add_actor(CLUTTER_CONTAINER(stage), video);

    clutter_actor_show_all(stage);

    clutter_main();

    return 0;
}

