#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <primary_data.h>

using namespace godot;

void ORC_PrimaryData::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_proxy_object"), &ORC_PrimaryData::get_proxy_object);
    ClassDB::bind_method(D_METHOD("set_proxy_object", "proxy_object"), &ORC_PrimaryData::set_proxy_object);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "proxy_object", PROPERTY_HINT_RESOURCE_TYPE, "ORC_ProxyObject"), "set_proxy_object", "get_proxy_object");

    ClassDB::bind_method(D_METHOD("get_secondary_data_array"), &ORC_PrimaryData::get_secondary_data);
    ClassDB::bind_method(D_METHOD("set_secondary_data", "secondary_data_array"), &ORC_PrimaryData::set_secondary_data);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "secondary_data_array", PROPERTY_HINT_ARRAY_TYPE, "ORC_SecondaryData"), "set_secondary_data", "get_secondary_data_array");
}
