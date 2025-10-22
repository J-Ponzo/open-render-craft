#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <proxy_object.h>

using namespace godot;

void ORC_ProxyObject::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_node"), &ORC_ProxyObject::get_node);
    ClassDB::bind_method(D_METHOD("set_node", "node"), &ORC_ProxyObject::set_node);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "node", PROPERTY_HINT_RESOURCE_TYPE, "Node"), "set_node", "get_node");

    ClassDB::bind_method(D_METHOD("get_primary_data"), &ORC_ProxyObject::get_primary_data);
    ClassDB::bind_method(D_METHOD("set_primary_data", "primary_data"), &ORC_ProxyObject::set_primary_data);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "primary_data", PROPERTY_HINT_RESOURCE_TYPE, "ORC_PrimaryData"), "set_primary_data", "get_primary_data");

    ClassDB::bind_method(D_METHOD("is_active"), &ORC_ProxyObject::is_active);
    ClassDB::bind_method(D_METHOD("set_active", "active"), &ORC_ProxyObject::set_active);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_active"), "set_active", "is_active");

    BIND_GD_OVERRIDABLE_METHOD(ORC_ProxyObject, update_data)
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_ProxyObject, void, update_data)
void ORC_ProxyObject::update_data_impl() {
    UtilityFunctions::print("ORC_ProxyObject.update_data");
}
