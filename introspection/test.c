#include <glib.h>
#include <girepository.h>
#include <telepathy-glib/telepathy-glib.h>

int main (int argc, char **argv)
{
    g_type_init();

    GIRepository *repository = g_irepository_get_default();

    GError *error = NULL;
    GTypelib *tp_type_lib = g_irepository_require(repository,
                                                  "TelepathyGLib",
                                                  NULL,
                                                  0,
                                                  &error);
    if (error) {
        g_error("Failed to load TP: %s", error->message);
        return 1;
    }

    GIBaseInfo *base_info = g_irepository_find_by_gtype(repository,
                                                        TP_TYPE_SIMPLE_OBSERVER);
    if (!base_info) {
        g_error("Did not find TpSimpleObserver.");
        return 1;
    }

    GIPropertyInfo *property_info = g_object_info_get_property((GIObjectInfo*)base_info, 0);
    g_debug("Name: %s", g_base_info_get_name((GIBaseInfo*)property_info));

    GITypeInfo *property_type = g_property_info_get_type(property_info);
    g_debug("Type tag: %s", g_type_tag_to_string(g_type_info_get_tag(property_type)));

    GITypeInfo *prop_interface_info = g_type_info_get_interface(property_type);
    GIInfoType prop_interface_type = g_base_info_get_type(prop_interface_info);
    if (prop_interface_type != GI_INFO_TYPE_CALLBACK) {
        g_error("NOT A CALLBACK");
        return 1;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    GIFunctionInfo *constructor_info = g_object_info_find_method((GIObjectInfo*)base_info, "new");
    g_debug("Name: %s", g_base_info_get_name((GIBaseInfo*)constructor_info));

    GICallableInfo *callable_info = (GICallableInfo*)constructor_info;
    GIArgInfo *arg_info = g_callable_info_get_arg(callable_info, 4);
    g_debug("Name: %s", g_base_info_get_name((GIBaseInfo*)arg_info));

    GITypeInfo *arg_type_info = g_arg_info_get_type(arg_info);
    g_debug("Type tag: %s", g_type_tag_to_string(g_type_info_get_tag(arg_type_info)));

    GITypeInfo *interface_info = g_type_info_get_interface(arg_type_info);
    GIInfoType interface_type = g_base_info_get_type(interface_info);
    if (interface_type != GI_INFO_TYPE_CALLBACK) {
        g_error("NOT A CALLBACK");
        return 1;
    }

    return 0;
}
