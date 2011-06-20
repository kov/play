#include <gtk/gtk.h>

static char* menu_definition = \
"\
<ui>\
  <popup>\
    <menuitem name=\"open\" action=\"openAction\" />\
  </popup>\
</ui>\
";

int main(int argc, char** argv)
{
  GtkWidget* menu;
  GtkUIManager* manager;

  gtk_init(&argc, &argv);

  manager = gtk_ui_manager_new();
  gtk_ui_manager_add_ui_from_string(manager, menu_definition, -1, NULL);

  menu = gtk_ui_manager_get_widget(manager, "/ui/popup");
  gtk_widget_show_all(menu);

  gtk_menu_popup(menu, NULL, NULL, 0, NULL, 0, gtk_get_current_event_time());

  gtk_main();

  return 0;
}
