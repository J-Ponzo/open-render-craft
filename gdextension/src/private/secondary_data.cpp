#include <godot_cpp/core/class_db.hpp>

#include <secondary_data.h>

using namespace godot;

void ORC_SecondaryData::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_primary_data"), &ORC_SecondaryData::get_primary_data);
    ClassDB::bind_method(D_METHOD("set_primary_data", "primary_data"), &ORC_SecondaryData::set_primary_data);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "primary_data", PROPERTY_HINT_RESOURCE_TYPE, "ORC_PrimaryData"), "set_primary_data", "get_primary_data");
}
